#include "masterSlave.h"

#include "Arduino.h"

 void MasterSlave::init(Communicator& communicator) {
    this->communicator = &communicator;

    uint8_t i = 0;
    for(i=0; i < COM_CHANNEL_COUNT; i++) {
        msChannel[i].init(i,&communicator);
    }
 }



 void MasterSlave::passToken(ComToken * token) {
    uint8_t i = 0;
    for(i=0; i < COM_CHANNEL_COUNT; i++) {
        msChannel[i].passToken(token);
    }
 }

 void MasterSlave::process(void) {
    uint8_t i = 0;
    for(i=0; i < COM_CHANNEL_COUNT; i++) {
        msChannel[i].process();
    }
 }






 void MasterSlave::setMasterPayload(uint8_t payload) {
     this->masterPayload = payload;
     uint8_t i = 0;
    for(i=0; i < COM_CHANNEL_COUNT; i++) {
        msChannel[i].setMasterPayload(payload);
    }
 }

MsChannel * MasterSlave::getChannel(uint8_t channel) const {
    return &msChannel[channel];
}  
