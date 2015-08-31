/* 
 * File:   sbs1_client.h
 * Author: clemensv
 *
 * Created on 26. August 2015, 17:17
 */

#ifndef SBS1_CLIENT_H
#define	SBS1_CLIENT_H

#include <time.h>


#ifdef	__cplusplus
extern "C" {
#endif

typedef struct tagAdsbRecord
{
    int transmissionType;
    char * icaoHexIdentifier;
    char * generatedIsoTime;
    char * callsign;
    int altitude;
    int groundSpeed;
    int groundTrackAngle;
    float latitude;
    float longitude;
    int verticalRate;
    char * squawk;    
} AdsbRecord;
    
typedef void (*processAdsbRecordCallback)(AdsbRecord * adsbRecord, void * context);    
    
int sbs1Client(char * hostName, int port, processAdsbRecordCallback * callback, void * context);


#ifdef	__cplusplus
}
#endif

#endif	/* SBS1_CLIENT_H */

