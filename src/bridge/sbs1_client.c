#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <malloc/malloc.h>

#include "sbs1_client.h"


const int STATE_INIT = 0;
const int STATE_FIELD = 1;
const int STATE_SEPARATOR = 2;
const int STATE_EOL = 3;
const int STATE_EOF = -1;

typedef struct tagParseState
{
    int state;
    int fieldNumber;
    char* spillover;
    char** fields;
} parseState;

int processSbs1Connection(int socketHandle, processAdsbRecordCallback callback, void * context);

int sbs1Client(char * hostName, int port, processAdsbRecordCallback * callback, void * context)
{
    struct sockaddr_in serv_addr;
    struct hostent *server;

    int socketHandle;

    socketHandle = socket(AF_INET, SOCK_STREAM, 0);
    if (socketHandle < 0)
    {
        return -1;
    }
    server = gethostbyname(hostName);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        return -2;
    }

    memset(&serv_addr, 0, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr, *(server->h_addr_list), sizeof (serv_addr.sin_addr));
    serv_addr.sin_port = htons(port);

    if (connect(socketHandle, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0)
    {
        close(socketHandle);
        return -3;
    }

    processSbs1Connection(socketHandle, callback, context);

    close(socketHandle);
    return 0;
}

parseState * newParseState()
{
    parseState * ps = malloc(sizeof (parseState));
    memset(ps, 0, sizeof (parseState));
    ps->state = STATE_INIT;
    ps->fieldNumber = -1;
    ps->spillover = NULL;
    ps->fields = malloc(sizeof (char*) * 255);
    return ps;
}

void resetParseState(parseState * ps)
{
    int i;
    for (i = 0; i < 255; i++)
    {
        if (ps->fields[i] != NULL)
        {
            free(ps->fields[i]);
            ps->fields[i] = NULL;
        }
    }
    if (ps->spillover != NULL)
    {
        free(ps->spillover);
        ps->spillover = NULL;
    }

    ps->state = STATE_INIT;
    ps->fieldNumber = -1;
}

void freeParseState(parseState * ps)
{
    resetParseState(ps);
    free(ps->fields);
    free(ps);
}

int processSbs1Connection(int socketHandle, processAdsbRecordCallback callback, void * context)
{
    int bufferSize = 1024;
    int bytesRead = 0;
    char * buffer = malloc(bufferSize);
    parseState * ps;

    ps = newParseState();

    while ((bytesRead = recv(socketHandle, buffer, bufferSize, 0)) > 0)
    {
        parseBuffer(buffer, bytesRead, ps, callback, context);
    }
    freeParseState(ps);

    free(buffer);
    return 0;
}

void handleCallback(processAdsbRecordCallback callback, void * context, char ** fields, int count)
{
    AdsbRecord record;

    memset(&record, 0, sizeof(record));
    if (count > 1 && fields[1] != NULL)
    {
        record.transmissionType = atoi(fields[1]);
    }
    if (count > 4 && fields[4] != NULL)
    {
        record.icaoHexIdentifier = fields[4];
    }
    if ( count > 9 && fields[8] != NULL && fields[9] != NULL)
    {
        record.generatedIsoTime = malloc(32);
        int y, m, d, hr, mi, sc, ms; 
       sscanf(fields[8], "%d/%d/%d", &y, &m, &d);
       sscanf(fields[9], "%d:%d:%d.%d", &hr, &mi,&sc, &ms);   
       sprintf(record.generatedIsoTime, "%04d-%02d-%02dT%02d:%02d:%02d.%dZ", y,m,d,hr,mi,sc,ms);
    }
    if (count > 10 && fields[10] != NULL)
    {
        record.callsign = fields[10];
    }
    if (count > 11 && fields[11] != NULL && fields[11][0] != 0x00)
    {
        record.altitude = atoi(fields[11]);
    }
    else
    {
        record.altitude = -1;
    }
    if (count > 12 && fields[12] != NULL && fields[12][0] != 0x00)
    {
        record.groundSpeed = atoi(fields[12]);
    }
    else 
    {
        record.groundSpeed = -1;
    }
    if (count > 13 && fields[13] != NULL && fields[13][0] != 0x00)
    {
        record.groundTrackAngle = atoi(fields[13]);
    }
    else 
    {
        record.groundTrackAngle = -1;
    }
    if (count > 14 && fields[14] != NULL && fields[14][0] != 0x00)
    {
        record.latitude = atof(fields[14]);
    }
    else 
    {
        record.latitude = -1;
    }
    if (count > 15 && fields[15] != NULL && fields[15][0] != 0x00)
    {
        record.longitude = atof(fields[15]);
    }
    else 
    {
        record.longitude = -1;
    }
    if (count > 16 && fields[16] != NULL && fields[16][0] != 0x00)
    {
        record.verticalRate = atoi(fields[16]);
    }
    else 
    {
        record.verticalRate = -1;
    }
    if (count > 17 && fields[17] != NULL)
    {
        record.squawk = fields[17];
    }
    
    callback( &record, context );
    
    if ( record.generatedIsoTime )
    {
        free( record.generatedIsoTime );
    }
}

int parseBuffer(char * buffer, int count, parseState * ps, processAdsbRecordCallback callback, void * context)
{
    char * mark = NULL;
    char * pos = buffer;
    char * end = buffer + count;

    while (pos < end)
    {
        if (*pos == ',' || *pos == 0x0A || *pos == 0x0D)
        {
            if (ps->state == STATE_INIT || ps->state == STATE_SEPARATOR)
            {
                ps->fieldNumber += 1;
            }
            if (ps->state == STATE_FIELD)
            {
                if (ps->spillover != NULL)
                {
                    if (mark == NULL)
                    {
                        ps->fields[++(ps->fieldNumber)] = strdup(ps->spillover);
                    }
                    else
                    {
                        int sol = strlen(ps->spillover);
                        char * val = (char*) malloc(pos - mark + sol);
                        strcpy(val, ps->spillover);
                        strncpy(val + sol, mark, pos - mark);
                        ps->fields[++(ps->fieldNumber)] = val;
                    }
                    free(ps->spillover);
                    ps->spillover = NULL;
                }
                else
                {
                    ps->fields[++(ps->fieldNumber)] = strndup(mark, pos - mark);
                }
                mark = NULL;
            }

            if (*pos == ',')
            {
                ps->state = STATE_SEPARATOR;
            }
            else if (*pos == 0x0A || *pos == 0x0D)
            {
                if (ps->state != STATE_EOL)
                {
                    handleCallback(callback, context, ps->fields, ps->fieldNumber);
                    resetParseState(ps);
                }
                ps->state = STATE_EOL;
            }
        }
        else
        {
            ps->state = STATE_FIELD;
            if (mark == NULL)
            {
                mark = pos;
            }
        }
        pos++;
    }
    if (ps->state == STATE_FIELD && mark != NULL)
    {
        ps->spillover = strndup(mark, pos - mark);
    }
}
