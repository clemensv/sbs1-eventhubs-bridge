
/*
Microsoft Azure IoT Device Libraries
Copyright (c) Microsoft Corporation
All rights reserved. 
MIT License
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the Software), to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
IN THE SOFTWARE.
*/

#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <stdio.h>
#include <stdlib.h>
#include <eventhubclient.h>
#include <eventdata.h>
#include <threadapi.h>
#include <crt_abstractions.h>


static EVENTHUBCLIENT_HANDLE eventHubClientHandle = NULL;



// have to factor this to create the EH client separately. Later.
int sendPayload(char * payload, char * contentType, char * connectionString, char * entityPath)
{
    int result = 0;

    if ( eventHubClientHandle == NULL )
    {
        eventHubClientHandle = EventHubClient_CreateFromConnectionString(connectionString, entityPath);
        if (eventHubClientHandle == NULL)
        {
            result = 1;
        }
    }
    
    EVENTDATA_HANDLE eventDataHandle = EventData_CreateWithNewMemory( (const unsigned char*)payload, strlen(payload));
    if (eventDataHandle == NULL)
    {
        result = 1;
    }
    else
    {
        if (EventHubClient_Send(eventHubClientHandle, eventDataHandle) != EVENTHUBCLIENT_OK)
        {
            result = 1;
        }
        else
        {
            result = 0;
        }
        EventData_Destroy(eventDataHandle);
    }
    return result; 
}
