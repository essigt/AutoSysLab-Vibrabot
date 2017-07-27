#include "SwarmBot.h"
#include "Arduino.h"

void testCompass2(void);

void SwarmBot::init(void) {
  Vibrabot::init();

  this->leftMotor.setPwmValue(120);
  this->rightMotor.setPwmValue(130);

  this->rgbLed.setLEDColor(&ColorOrange);
  this->masterSlave.init(this->communicator);

  this->masterSlave.getChannel(0)->setActAsSlave(true);
  this->masterSlave.getChannel(3)->setActAsMaster(true);
}


void SwarmBot::process(void)
{
  ComToken * token;

  if (this->mainClock.isTick())
  {
    this->rgbLed.process();
    this->processAnalogData();

    bool alreadyProcessed= false;


    if (this->communicator.hasTokenToRead())
    {
      token =  this->communicator.readNextToken();

      if (token->getHeader() == MS_TOKEN_HEADER) {
        uint8_t payload = token->getData() >> 2;

        if (this->masterSlave.getChannel(0)->getMode() == MS_MODE_SLAVE && token->getChannel() == 0) {
          motorRunRecived = payload;
          this->masterSlave.setMasterPayload( motorRunRecived );
          this->masterSlave.process();
          alreadyProcessed = true;
        }

        this->masterSlave.passToken(token);
      }
    }


    
    //Toggle LED every 5sec
    if (counter == 5000) {
      if (this->masterSlave.getChannel(3)->getMode() == MS_MODE_MASTER && this->masterSlave.getChannel(0)->getMode() == MS_MODE_SINGLE ) {
        motorRunSend = !motorRunSend;
        this->masterSlave.setMasterPayload( motorRunSend );
      }
      counter = 0;
    }
    counter++;

    

    if(!alreadyProcessed)
      this->masterSlave.process();
    
    this->updateLED();
  }
}



void SwarmBot::updateLED(void)
{
  bool motorOn = false;
  static uint8_t ledFlashCounter = 0;

  this->rgbLed.setLEDColor(&ColorBlue);

  if (this->masterSlave.getChannel(0)->getMode() == MS_MODE_SINGLE && this->masterSlave.getChannel(3)->getMode() == MS_MODE_SINGLE) {
    this->rgbLed.setLEDColor(&ColorGreen);

    motorOn = ledFlashCounter < 127;
    ledFlashCounter++;

  } else if (this->masterSlave.getChannel(3)->getMode() == MS_MODE_MASTER && this->masterSlave.getChannel(0)->getMode() == MS_MODE_SLAVE) { //I am a Master and Slave: Use received token
    this->rgbLed.setLEDColor(&ColorPurple);

    motorOn = motorRunRecived;

  } else if (this->masterSlave.getChannel(0)->getMode() == MS_MODE_SLAVE) {
    this->rgbLed.setLEDColor(&ColorRed);

    motorOn = motorRunRecived;

  } else if (this->masterSlave.getChannel(3)->getMode() == MS_MODE_MASTER) {
    this->rgbLed.setLEDColor(&ColorBlue);

    motorOn = motorRunSend ;

  }



  if (motorOn) {
    //this->leftMotor.on();
    //this->rightMotor.on();
    this->redLed.ledOn();
  } else {
    //this->leftMotor.off();
    //this->rightMotor.off();
    this->redLed.ledOff();
  }
}

