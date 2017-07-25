
#include "SwarmBot.h"
#include "Arduino.h"


void SwarmBot::init(void) {
  Vibrabot::init();
  
  this->leftMotor.setPwmValue(120);
  this->rightMotor.setPwmValue(120);
  
  this->rgbLed.setLEDColor(&ColorOrange);
}

void SwarmBot::process(void)
{
  //  adc.dump();



  if (this->mainClock.isTick())
  {

    this->rgbLed.process();
    this->processAnalogData();


    //Master/Slave Proof of Concept
    this->sendMsg();
    this->checkMaster();


    
    this->checkLight();

    this->testCompass();
    
    if (counter == 1000) {
      counter = 0;
    }
    counter++;

  }


}


extern void printData(char * text, uint16_t data);


void SwarmBot::checkMaster(void) {

  ComToken * token2;

  if (this->communicator.hasTokenToRead())
  {
    token2 =  this->communicator.readNextToken();

    if (token2->getChannel() == 0) {
      Serial.print("Master Token received");
      Serial.print(token2->getHeader());
      Serial.print(" : ");
      Serial.print(token2->getData());

      Serial.print(" C : ");
      Serial.print(token2->getChannel());

      Serial.print(" S: ");
      Serial.println(token2->getStrength());


      noMasterTokenCounter = 1500;
      state = STATE_SLAVE;
    }


  } else {

    noMasterTokenCounter--;
    if (noMasterTokenCounter == 0) {
      state = STATE_MASTER;
    }
  }


}

void SwarmBot::sendMsg(void) {
  if (counter % 500 == 0) { //twice a second


    //Send Message to Slave
    ComToken  token;
    token.init(COM_TOKEN_RESPONSE);
    token.setHeader(0x01);
    token.setData(0x02);


    /*Serial.print("Token send :");
      Serial.print(token.getHeader());
      Serial.print(" : ");
      Serial.println(token.getData());*/

    this->communicator.send(0x08, token);
    Serial.println("Master token send");

  }
}


void SwarmBot::checkLight(void)
{
  uint16_t lightLeft;
  uint16_t lightRight;

  lightLeft = this->adc.getBrightness(0);
  lightRight = this->adc.getBrightness(1);


  if (state == STATE_SLAVE) {
    this->rgbLed.setLEDColor(&ColorBlue);

  } else {
    this->rgbLed.setLEDColor(&ColorRed);



  }


  /*

    if (lightLeft > ((lightRight * 18) / 16))
    {
      Serial.println("turn right");

        this->leftMotor.on();
        this->rightMotor.off();

    }
    else   if (lightRight > ((lightLeft * 18) / 16))
    {
      Serial.println("turn left");
        this->leftMotor.off();
        this->rightMotor.on();
    }
    else
    {


       this->leftMotor.off();
        this->rightMotor.off();
    }*/
  //printData("Left: ",this->adc.getLightValue(0));
  //printData("Right: ",this->adc.getLightValue(1));

}


void SwarmBot::executeState(void)
{


}


