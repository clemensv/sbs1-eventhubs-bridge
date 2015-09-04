/* 
 * File:   eventhub_client.h
 * Author: clemensv
 *
 * Created on 31. August 2015, 07:37
 */

#ifndef EVENTHUB_CLIENT_H
#define	EVENTHUB_CLIENT_H

#ifdef	__cplusplus
extern "C" {
#endif

void sendPayload(char * payload, char * contentType, char * connectionString, char * entityPath);


#ifdef	__cplusplus
}
#endif

#endif	/* EVENTHUB_CLIENT_H */

