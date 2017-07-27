/**
    File: masterSlave.h
    Authors: Tim Essig
    Version: 0.4.2 beta 2017-07-25

    Description: Header file for the MasterSlave module.
                This file contains the definitions of the MasterSlave module
                The MasterSlave module provices functionality for connection the bots in master-slave mode 
                The process-method has to be called once every tick.
                The Master-Tokens has to be passed by the application.
*/


#ifndef VIBRABOT_FIRMWARE_MODULE_MASTER_SLAVE_H
#define VIBRABOT_FIRMWARE_MODULE_MASTER_SLAVE_H

#include "modules/com/inc/communicator.h"

#include "msConfig.h"
#include "msChannel.h"

class MasterSlave {
    private:
        Communicator * communicator;
        MsChannel msChannel [COM_CHANNEL_COUNT];
        
       
        uint8_t sendDirection;
        uint8_t receiveDirection;

        uint8_t masterPayload = 0; // 6 Bit

        uint16_t tickCounter = 0;

        

    public:
        void init(Communicator& communicator);
        void passToken(ComToken * token);

        /**
        * 
        */ 
        void process(void);

        

        void setMasterPayload(uint8_t payload);
        MsChannel * getChannel(uint8_t channel) const;
};


#endif
