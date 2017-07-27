#ifndef VIBRABOT_FIRMWARE_MODULE_MS_CHANNEL_H
#define VIBRABOT_FIRMWARE_MODULE_MS_CHANNEL_H

#include "modules/com/inc/communicator.h"

#include "msConfig.h"
#include "Arduino.h"


class MsChannel {

private:
    Communicator * communicator;

    uint8_t channel;

     bool actAsMaster = false;
     bool actAsSlave = false;

     uint8_t masterPayload = 0; // 6 Bit

    uint8_t mode = MS_MODE_SINGLE;
    uint16_t connectionToMasterTimeout = 0;
    uint16_t connectionToSlaveTimeout = 0;

    uint16_t tickCounter = 0;

    ComToken token;
    void sendMasterToken(void);

public:
    void init(uint8_t channel, Communicator * communicator);
    void passToken(ComToken * token);

    /**
    * 
    */ 
    void process(void);

    /**
    * Returns the current mode of the bot
    */
    uint8_t getMode(void) const;

    void setMasterPayload(uint8_t payload);

    void setActAsMaster(bool actAsMaster);
    void setActAsSlave(bool actAsSlave);
};

#endif
