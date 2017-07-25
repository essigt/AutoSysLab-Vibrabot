
#include "SwarmBot.h"
#include "Arduino.h"

void testCompass2(void);

void SwarmBot::init(void) {
  Vibrabot::init();

  this->leftMotor.setPwmValue(120);
  this->rightMotor.setPwmValue(120);

  this->rgbLed.setLEDColor(&ColorOrange);

  this->masterSlave.init(this->communicator);

}


void SwarmBot::process(void)
{
  ComToken * token;



  if (this->mainClock.isTick())
  {


    this->rgbLed.process();
    this->processAnalogData();


    this->masterSlave.process();


    if (this->communicator.hasTokenToRead())
    {
      token =  this->communicator.readNextToken();
      Serial.print("C:");
      Serial.print(token->getChannel());
      Serial.print(" Sig:");
      Serial.println(token->getStrength());
      
      
      //TODO Only pass token if it is a MasterToken
      if (token->getHeader() == MS_TOKEN_HEADER) {
        this->masterSlave.passToken(token);
      }
    }

    this->updateLED();


  }
}


void SwarmBot::updateLED(void)
{
  uint16_t lightLeft;
  uint16_t lightRight;

  lightLeft = this->adc.getBrightness(0);
  lightRight = this->adc.getBrightness(1);

  this->rgbLed.setLEDColor(&ColorBlue);

  if (this->masterSlave.getMode() == MS_MODE_SINGLE) {
    this->rgbLed.setLEDColor(&ColorGreen);
    this->leftMotor.off();
    this->rightMotor.off();
  } else if (this->masterSlave.getMode() & MS_MODE_MASTER && this->masterSlave.getMode() & MS_MODE_SLAVE) {
    this->rgbLed.setLEDColor(&ColorBlue);
    this->leftMotor.on();
    this->rightMotor.on();
  } else if (this->masterSlave.getMode() & MS_MODE_SLAVE) {
    this->rgbLed.setLEDColor(&ColorRed);
    this->leftMotor.off();
    this->rightMotor.off();
  } else if (this->masterSlave.getMode() & MS_MODE_MASTER) {
    this->leftMotor.on();
    this->rightMotor.on();    
  }


}



