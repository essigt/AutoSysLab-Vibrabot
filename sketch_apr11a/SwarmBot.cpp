
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

        uint8_t payload = token->getData() >> 4;
        Serial.print("P: ");
        Serial.println(payload);

        motorRunRecived = payload;
        
      }
    }



    this->updateLED();


    if(counter == 2000) {
      counter = 0;

      Serial.print("SendP: ");
      Serial.println(motorRunSend);

      Serial.print("Mode: ");
      Serial.println(this->masterSlave.getMode() );

      Serial.print("Mode enter: ");
      Serial.println(this->masterSlave.getMode() == MS_MODE_MASTER );
         
      if (this->masterSlave.getMode() == MS_MODE_MASTER ) {
        motorRunSend = !motorRunSend;
         this->masterSlave.setMasterPayload( motorRunSend );
         Serial.print("SendP: ");
         Serial.println(motorRunSend);
      }
    }

    counter++;

  }
}



void SwarmBot::updateLED(void)
{
  bool motorOn = false;

  this->rgbLed.setLEDColor(&ColorBlue);

  if (this->masterSlave.getMode() == MS_MODE_SINGLE) {
    this->rgbLed.setLEDColor(&ColorGreen);
    //motorOn = true;

  } else if (this->masterSlave.getMode() & MS_MODE_MASTER && this->masterSlave.getMode() & MS_MODE_SLAVE) {
    this->rgbLed.setLEDColor(&ColorPurple);

    motorOn = motorRunRecived;

  } else if (this->masterSlave.getMode() & MS_MODE_SLAVE) {
    this->rgbLed.setLEDColor(&ColorRed);

    motorOn = motorRunRecived;

  } else if (this->masterSlave.getMode() & MS_MODE_MASTER) {
    this->rgbLed.setLEDColor(&ColorBlue);

    motorOn = motorRunSend;

  }

  
  
  if (motorOn) {
    this->leftMotor.on();
    this->rightMotor.on();
    this->redLed.ledOn();
  } else {
    this->leftMotor.off();
    this->rightMotor.off();
    this->redLed.ledOff();
  }
}


void SwarmBot::receiveLEDColor(void) {
  static uint8_t pos = 0;
  static uint8_t r=0, g=0, b=0;
  
  int incomingByte = 0;
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    if(pos == 0) {
      r = incomingByte - '0';
    }
    if(pos == 1) {
      g = incomingByte - '0';
    }
    if(pos == 2) {
      b = incomingByte - '0';
    }

    this->rgbLed.setLEDColor(r << 6, g << 6,  b << 6);

    pos++;
    if(pos == 3) {
      pos = 0;
    }
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte- '0', DEC);
  }
}

