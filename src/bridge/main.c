// put MIT license here

#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>

#include "sbs1_client.h"
#include "eventhub_client.h"

static int payloadBufferSize = 8192;
static int isRunning = 0;
static int delay = 1;
static int counter = 0;
static char *configFileName = NULL;
static char *lockFileName = NULL;
static char *sbs1ServerAddress = "localhost";
static char *amqpAddress = NULL;
static char *sasToken = NULL;
static char *sasRule = NULL;
static char *sasKey = NULL;
static char *connectionString = NULL;

static int lockFileHandle = -1;
static char *serviceName = NULL;
static FILE *userLogStream;

typedef struct tagMessagePayloadContext
{
    char * buffer;
    int bufferSize;

} MessagePayloadContext;

int readConfig(bool refreshConfig)
{

    FILE *configFile = NULL;
    int returnCode = -1;

    if (configFileName == NULL)
    {
        return 0;
    }

    configFile = fopen(configFileName, "r");
    if (configFile == NULL)
    {
        syslog(LOG_ERR, "Unable to open configuration file: %s. Error: %s", configFileName, strerror(errno));
        return -1;
    }

    returnCode = fscanf(configFile, "%d", &delay);
    if (returnCode > 0)
    {
        if (refreshConfig)
        {
            syslog(LOG_INFO, "Refreshed configuration file %s for service %s", configFileName, serviceName);
        }
        else
        {
            syslog(LOG_INFO, "Loaded configuration file %s for service %s", configFileName, serviceName);
        }
    }
    fclose(configFile);

    return returnCode;
}

void handleProcessSignal(int sig)
{
    if (sig == SIGINT)
    {
        fprintf(userLogStream, "Stopping daemon ...\n");
        /* Unlock and close lockfile */
        if (lockFileHandle != -1)
        {
            lockf(lockFileHandle, F_ULOCK, 0);
            close(lockFileHandle);
        }
        /* Try to delete lockfile */
        if (lockFileName != NULL)
        {
            unlink(lockFileName);
        }
        isRunning = 0;
        /* Reset signal handling to default behavior */
        signal(SIGINT, SIG_DFL);
    }
    else if (sig == SIGHUP)
    {
        fprintf(userLogStream, "Reloading configuration ...\n");
        readConfig(1);
    }
    else if (sig == SIGCHLD)
    {
        fprintf(userLogStream, "Debug: received SIGCHLD signal\n");
    }
}

static void forkDaemonProcess()
{
    pid_t processId = 0;
    int fileHandle;

    processId = fork();
    if (processId < 0)
    {
        exit(EXIT_FAILURE);
    }
    // only the parent knows the processId and exits
    if (processId > 0)
    {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0)
    {
        exit(EXIT_FAILURE);
    }
    signal(SIGCHLD, SIG_IGN);
    processId = fork();
    if (processId < 0)
    {
        exit(EXIT_FAILURE);
    }
    if (processId > 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    chdir("/");

    for (fileHandle = sysconf(_SC_OPEN_MAX); fileHandle > 0; fileHandle--)
    {
        close(fileHandle);
    }

    stdin = fopen("/dev/null", "r");
    stdout = fopen("/dev/null", "w+");
    stderr = fopen("/dev/null", "w+");

    if (lockFileName != NULL)
    {
        char * str = malloc(256);
        lockFileHandle = open(lockFileName, O_RDWR | O_CREAT, 0640);
        if (lockFileHandle < 0)
        {
            exit(EXIT_FAILURE);
        }
        if (lockf(lockFileHandle, F_TLOCK, 0) < 0)
        {
            exit(EXIT_FAILURE);
        }
        snprintf(str, 256, "%d\n", getpid());
        write(lockFileHandle, str, strlen(str));
        free(str);
    }
}


static char * csvHeader = "ttyp,time,icao,call,alt,gspd,gtrk,lat,lon,sqwk\n";

void processDataRecord(AdsbRecord * record, MessagePayloadContext * context)
{
    char stringBuffer[256];

    snprintf(stringBuffer, sizeof (stringBuffer),
             "%d,%s,%s,%s,%d,%d,%d,%f,%f,%s\n",
             record->transmissionType,
             record->generatedIsoTime,
             record->icaoHexIdentifier,
             record->callsign != NULL ? record->callsign : "",
             record->altitude,
             record->groundSpeed,
             record->groundTrackAngle,
             record->latitude,
             record->longitude,
             record->squawk != NULL ? record->squawk : "");

    if (strlen(context->buffer) + strlen(stringBuffer) > context->bufferSize - 1)
    {
        // flush out and realloc
        sendPayload(context->buffer, "text/csv", connectionString, amqpAddress);
        strcpy(context->buffer, csvHeader);
        strcat(context->buffer, stringBuffer);
    }
    else
    {
        strcat(context->buffer + strlen(context->buffer), stringBuffer);
    }
}

void printHelp(void)
{
    printf("\n Usage: %s [OPTIONS]\n\n", serviceName);
    printf("  Options:\n");
    printf("   -h --help               Print this help\n");
    printf("   -s --sbs <addr>:<port>  IP address or hostname and port of the SBS-1 logger\n" \
           "                           [defaults to localhost:30003 if not specified]\n");
    printf("   -x  --cxnstring \"<cxs>\" Azure Service Bus connection string, in quotation marks.\n" \
                                       "When provided, -q must be a relative entity path.\n");
    printf("   -q --queue <amqp-url>   AMQP(S) URL of the target to receive the event\n" \
           "                           data, e.g. amqps://myns.servicebus.windows.net/myeh\n");
    printf("   -t --token <token>      Claim-based security token. A SAS token for Azure\n" \
           "                           Service Bus\n");
    printf("   -S --sasrule <name>     Name of the SB shared access rule (not with --token)\n");
    printf("   -K --saskey <key>       Name of the SB shared access key (not with --token)\n");
    printf("   -c --cfg filename       Configuration file\n");
    printf("   -l --log  filename      Log file\n");
    printf("   -d --daemon             Run as background daemon\n");
    printf("   -m --mutex  filename    Mutex (PID) file for daemon\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    static struct option long_options[] = {
        {"cfg", required_argument, 0, 'c'},
        {"log", required_argument, 0, 'l'},
        {"help", no_argument, 0, 'h'},
        {"daemon", no_argument, 0, 'd'},
        {"sbs", required_argument, 0, 's'},
        {"queue", required_argument, 0, 'q'},
        {"token", required_argument, 0, 't'},
        {"sasrule", required_argument, 0, 'S'},
        {"saskey", required_argument, 0, 'K'},
        {"cxnstring", required_argument, 0, 'x'},
        {"mutex", required_argument, 0, 'm'},
        {NULL, 0, 0, 0}
    };
    int value, optionIndex = 0, ret;
    char *logFileName = NULL;
    int runAsDaemon = 0;
    int portNumber = 30003;

    serviceName = argv[0];

    while ((value = getopt_long(argc, argv, "c:l:h:d:s:q:t::S:K:x:m", long_options, &optionIndex)) != -1)
    {
        switch (value)
        {
            case 'c':
                configFileName = strdup(optarg);
                break;
            case 'l':
                logFileName = strdup(optarg);
                break;
            case 'm':
                lockFileName = strdup(optarg);
                break;
            case 'd':
                runAsDaemon = 1;
                break;
            case 'h':
                printHelp();
                return EXIT_SUCCESS;
            case 's':
            {
                char * split = NULL;
                sbs1ServerAddress = strdup(optarg);
                for (split = sbs1ServerAddress; *split != ':' && *split != 0x00; split++);
                if (*split == ':')
                {
                    // port number is whatever follows the colon
                    portNumber = atoi(++split);
                }
            }
                break;
            case 'q':
                amqpAddress = strdup(optarg);
                break;
            case 't':
                sasToken = strdup(optarg);
                break;
            case 'S':
                sasRule = strdup(optarg);
                break;
            case 'K':
                sasKey = strdup(optarg);
                break;
            case 'x':
                connectionString = strdup(optarg);
                break;
            case '?':
                printHelp();
                return EXIT_FAILURE;

            default:
                break;

        }
    }

    if ((amqpAddress == NULL && connectionString == NULL) ||
        (connectionString == NULL && (sasToken == NULL ^ (sasKey == NULL || sasRule == NULL))))
    {
        printf("The fully qualified AMQP target address must be given with credentials or "\
               "a connection string for Azure Service Bus along with a relative AMQP "\
               "address indicating the target entity");
        return EXIT_FAILURE;
    }

    if (connectionString == NULL &&
        strncmp(amqpAddress, "amqp://", 7) != 0 &&
        strncmp(amqpAddress, "amqps://", 8) != 0)
    {
        printf("Invalid AMQP address '%s'\n", amqpAddress);
        return EXIT_FAILURE;
    }


    if (sasToken != NULL && (sasRule != NULL || sasKey != NULL))
    {
        printf("-S or -K cannot be specified with -t\n");
        return EXIT_FAILURE;
    }

    if (runAsDaemon == 1)
    {
        forkDaemonProcess();
    }

    openlog(serviceName, LOG_PID | LOG_CONS, LOG_DAEMON);
    syslog(LOG_INFO, "Running %s", serviceName);

    signal(SIGINT, handleProcessSignal);
    signal(SIGHUP, handleProcessSignal);

    if (logFileName != NULL)
    {
        userLogStream = fopen(logFileName, "a+");
        if (userLogStream == NULL)
        {
            syslog(LOG_ERR, "Can not open log file: %s, error: %s", logFileName, strerror(errno));
            userLogStream = stdout;
        }
    }
    else
    {
        userLogStream = stdout;
    }

    readConfig(false);

    MessagePayloadContext ctx;
    ctx.buffer = malloc(payloadBufferSize);
    ctx.bufferSize = payloadBufferSize;
    strcpy(ctx.buffer, csvHeader);

    isRunning = 1;
    while (isRunning)
    {
        // if the socket drops, we're just going to go again.
        sbs1Client(sbs1ServerAddress, portNumber, &processDataRecord, &ctx);
    }

    if (userLogStream != stdout)
    {
        fclose(userLogStream);
    }

    syslog(LOG_INFO, "Stopped %s", serviceName);
    closelog();

    if (configFileName != NULL) free(configFileName);
    if (logFileName != NULL) free(logFileName);
    if (lockFileName != NULL) free(lockFileName);

    return EXIT_SUCCESS;
}