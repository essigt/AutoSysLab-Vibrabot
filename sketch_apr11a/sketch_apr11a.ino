#include <Vibrabot.h>




#include <Arduino.h>

#include <Wire.h>
#include "SwarmBot.h"


// initialize library class
SwarmBot bot;



void setup() {

 Serial.begin(115200);
  delay(2000); //Needed by UART to be ready
  bot.init();

}



void LEDTest()
{
    bot.process();
}


   

void loop() {

  LEDTest();



/*
  Wire.requestFrom(2, 6);    // request 6 bytes from slave device #2

  while(Wire.available())    // slave may send less than requested
  {
    char c = Wire.read();    // receive a byte as character
    Serial.print(c);         // print the character
  }

  delay(500);*/
  
}




