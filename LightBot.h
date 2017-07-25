
#include "Vibrabot.h"



typedef enum botState_e {BOT_STATE_MOVING_LEFT, BOT_STATE_MOVING_RIGHT, BOT_STATE_RESTING};

const uint8_t STATE_MASTER = 0x01;
const uint8_t STATE_SLAVE = 0x02;

class LightBot : public Vibrabot {

protected:
//  botState_e state;

   uint16_t counter = 0;
   uint16_t noMasterTokenCounter = 3000;

   uint8_t state = STATE_MASTER;
public:
  

  void init(void);
  void process(void);
  
  protected:
  void checkMaster(void);
  void sendMsg(void);
  void checkLight(void);
  void executeState(void);

};
