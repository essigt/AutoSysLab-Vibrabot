
#ifndef MSCONFIG_H_
#define MSCONFIG_H_

#include "modules/com/inc/comConfig.h"

#define MS_CHANNEL_COUNT        COM_CHANNEL_COUNT

#define MS_MODE_SINGLE          0x00
#define MS_MODE_MASTER          0x01
#define MS_MODE_SLAVE           0x02

#define MS_CONNECTION_TIMEOUT   2000 //Ticks

#define MS_TOKEN_HEADER         0x01

#define MS_SEND_TOKEN_INTERVAL  500  //Ticks

#define MS_BACK_CH              0x03
#define MS_FRONT_CH             0x00

#endif /* COMCONFIG_H_ */
