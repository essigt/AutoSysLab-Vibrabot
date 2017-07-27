#include "msChannel.h"

void MsChannel::init(uint8_t channel, Communicator * communicator) {
    this->channel = channel;
    this->communicator = communicator;
}


void MsChannel::passToken(ComToken * token) {
    if(!actAsMaster && !actAsSlave) {
        return; //Nothing to do, channel is idle
    }

     if( token->getChannel() == channel && token->getHeader() == MS_TOKEN_HEADER && token->isCommand() && actAsSlave ) {
         //Reset Timeout
        connectionToMasterTimeout = MS_CONNECTION_TIMEOUT;
        if(!(mode & MS_MODE_SLAVE)) {
            Serial.println("Slave");
        }
        mode |= MS_MODE_SLAVE;

        //Respond to master
        ComToken token2;
        token2.init(COM_TOKEN_RESPONSE);
        token2.setHeader(MS_TOKEN_HEADER);
        token2.setData(channel); //TODO: The IR Transmitter from with the Token has been send
        this->communicator->send( 0x01 << channel, token2); //Send from IR Transmitter 0

     } else  if( token->getChannel() == channel && token->getHeader() == MS_TOKEN_HEADER && token->isResponse() && actAsMaster) {
        connectionToSlaveTimeout = MS_CONNECTION_TIMEOUT;

        if(!(mode & MS_MODE_MASTER)) {
            Serial.println("Master");
        }
        mode |= MS_MODE_MASTER;
        
     }
 }

 void MsChannel::process(void) {
    //MASTER Part
     if(this->tickCounter == MS_SEND_TOKEN_INTERVAL) {
        if(actAsMaster) {
            //Send Tokens in "sendDirection" directions
            this->sendMasterToken();
        }

         this->tickCounter = 0;
     }
    this->tickCounter++;

    //Countdown timeout
     if(mode & MS_MODE_MASTER) {
         connectionToSlaveTimeout--;

         if(connectionToSlaveTimeout == 0) {
             mode &= ~MS_MODE_MASTER;
         }
     }

     



    //SLAVE Part
    if(actAsSlave) {
        //Countdown timeout
        if(mode & MS_MODE_SLAVE) {
            connectionToMasterTimeout--;

            if(connectionToMasterTimeout == 0) { //Connection timeout -> no longer a slave -> freeeeedom!!!
                mode &= ~MS_MODE_SLAVE;
            }
        }
    }
    
 }


 void MsChannel::sendMasterToken(void) {
    token.init(COM_TOKEN_COMMAND);

    token.setHeader(MS_TOKEN_HEADER);
    token.setData( this->masterPayload << 2 |channel); //TODO: The IR Transmitter from with the Token has been send

    this->communicator->send(0x01 << channel, token); //Send from IR Transmitter [channel]
 }

  uint8_t MsChannel::getMode(void) const {
     return(mode);
 }

 void MsChannel::setActAsMaster(bool actAsMaster) {
    this->actAsMaster = actAsMaster;
 }

 void MsChannel::setMasterPayload(uint8_t payload) {
     this->masterPayload = payload;
     this->tickCounter = MS_SEND_TOKEN_INTERVAL; //Force sending the mastertoken again
 }

void MsChannel::setActAsSlave(bool actAsSlave) {
    this->actAsSlave = actAsSlave;
}
