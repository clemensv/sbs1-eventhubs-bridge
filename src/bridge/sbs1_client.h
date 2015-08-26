/* 
 * File:   sbs1_client.h
 * Author: clemensv
 *
 * Created on 26. August 2015, 17:17
 */

#ifndef SBS1_CLIENT_H
#define	SBS1_CLIENT_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef void (*processDataRowCallback)(char** data, int rows);    
    
int sbs1Client(char * hostName, int port, processDataRowCallback * callback);


#ifdef	__cplusplus
}
#endif

#endif	/* SBS1_CLIENT_H */

