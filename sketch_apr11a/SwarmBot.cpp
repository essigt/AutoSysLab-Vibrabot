
#include "SwarmBot.h"
#include "Arduino.h"

void testCompass2(void);

void SwarmBot::init(void) {
  Vibrabot::init();


  //Paramter for white bot: TODO
  this->leftMotor.setPwmValue(130);
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

    if (counter == 2000) {
      counter = 0;

      if (this->masterSlave.getChannel(3)->getMode() == MS_MODE_MASTER && this->masterSlave.getChannel(0)->getMode() == MS_MODE_SINGLE ) {
        motorRunSend = !motorRunSend;
        this->masterSlave.setMasterPayload( motorRunSend );
      }
    }
    counter++;

    this->masterSlave.process();



    if (this->communicator.hasTokenToRead())
    {
      token =  this->communicator.readNextToken();
      
      if (token->getHeader() == MS_TOKEN_HEADER) {
        uint8_t payload = token->getData() >> 2;


        motorRunRecived = payload == 1 ? true : false;

        if(this->masterSlave.getChannel(0)->getMode() == MS_MODE_SLAVE ) {
          this->masterSlave.setMasterPayload( motorRunRecived );
        }
        this->updateLED();



        this->masterSlave.passToken(token);

      }
    }



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

  } else if (this->masterSlave.getChannel(3)->getMode() == MS_MODE_MASTER && this->masterSlave.getChannel(0)->getMode() == MS_MODE_SLAVE) {
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


void SwarmBot::receiveLEDColor(void) {
  static uint8_t pos = 0;
  static uint8_t r = 0, g = 0, b = 0;

  int incomingByte = 0;
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    if (pos == 0) {
      r = incomingByte - '0';
    }
    if (pos == 1) {
      g = incomingByte - '0';
    }
    if (pos == 2) {
      b = incomingByte - '0';
    }

    this->rgbLed.setLEDColor(r << 6, g << 6,  b << 6);

    pos++;
    if (pos == 3) {
      pos = 0;
    }
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte - '0', DEC);
  }
}

