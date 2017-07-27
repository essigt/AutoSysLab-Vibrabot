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

}




