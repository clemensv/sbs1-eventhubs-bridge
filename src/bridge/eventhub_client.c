
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

#include <stdio.h>
#include <stdlib.h>
#include <eventhubclient.h>
#include <eventdata.h>
#include <threadapi.h>
#include <crt_abstractions.h>

//static const char* connectionString = "Endpoint=sb://[namespace].servicebus.windows.net/;SharedAccessKeyName=[key name];SharedAccessKey=[key value]";
//static const char* eventHubPath = "[event hub name]";
static const char* connectionString = "Endpoint=sb://clemensveu.servicebus.windows.net/;SharedAccessKeyName=RootManageSharedAccessKey;SharedAccessKey=bIeXN/RZeANw59xt39ZGyOScYxqRe73NIT4ZxrDooj4=";
static const char* eventHubPath = "iotev1";



static bool g_bSendProperties = false;
static bool g_bSendPartitionKey = false;

#define BUFFER_SIZE     128


static EVENTHUBCLIENT_HANDLE eventHubClientHandle = NULL;


int sendPayload(char * payload, char * contentType)
{
    int result = 0;

    if ( eventHubClientHandle == NULL )
    {
        eventHubClientHandle = EventHubClient_CreateFromConnectionString(connectionString, eventHubPath);
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
