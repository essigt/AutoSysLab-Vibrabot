#include "Vibrabot.h"

#include "masterSlave.h"


class SwarmBot : public Vibrabot {

protected:

  uint16_t counter = 0;
  bool motorRunSend = false;
  bool motorRunRecived = false;

    MasterSlave masterSlave;
public:
  

  void init(void);
  void process(void);
  
 protected:
  void updateLED(void);
  void receiveLEDColor(void);

};
