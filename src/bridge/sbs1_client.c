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

int processSbs1Connection(int socketHandle, processDataRowCallback callback);


int sbs1Client(char * hostName, int port, processDataRowCallback * callback)
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

    processSbs1Connection(socketHandle, callback);

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


int processSbs1Connection(int socketHandle, processDataRowCallback callback)
{
    int bufferSize = 1024;
    int bytesRead = 0;
    char * buffer = malloc(bufferSize);
    parseState * ps;

    ps = newParseState();

    while ((bytesRead = recv(socketHandle, buffer, bufferSize, 0)) > 0)
    {
        parseBuffer(buffer, bytesRead, ps, callback);
    }
    freeParseState(ps);

    free(buffer);
    return 0;
}



int parseBuffer(char * buffer, int count, parseState * ps, processDataRowCallback callback)
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
                    callback(ps->fields, ps->fieldNumber);
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
