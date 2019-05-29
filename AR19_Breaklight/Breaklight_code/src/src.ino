/*
Title: ACM Brakelight Controller
Description: Main code for brakelight ACM
v 1.0
Last Revision Date: 06.04.2019

Created by Simon Nylund on 05.04.2019
Copyright © 2019 Align Racing UiA. All rights reserved.
*/


//Libraries
#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>
#include "AllSettings.h"

MCP2515 mcp2515(7);
struct can_frame canMsg;

int breakpressurepercent = 0;
bool lightsON = false;

void setup() {
 SPI.begin();
  
  //Debug led setup
  pinMode(rled, OUTPUT);
  pinMode(gled, OUTPUT);
  pinMode(bled, OUTPUT);
  digitalWrite(rled, HIGH);
  digitalWrite(gled, HIGH);
  digitalWrite(bled, HIGH);


  //CAN setup
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();

  //Brakelight setup
  pinMode(breaklightout, OUTPUT);
  digitalWrite(breaklightout, LOW);


}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    
    if(canMsg.can_id == breakPressureCANID){ //Henter ut data fra riktig CAN melding

      //breakPressure1 = canMsg.data[0]; //Trykk i bremsekrets 1 (35 bar)
      breakPressure = canMsg.data[1]; //Trykk i bremsekrets 2 (100 bar)

    }else{}


    if (breakPressure > pressureThresholdOn) { //Skrur på bremselyset hvis den går forbi bremsethreshold (Vedien 0-255)
        lightsON = true;
    }
    else if (breakPressure < pressureThresholdOff ) { //Skrur på bremselyset hvis den går forbi bremsethreshold (Vedien 0-255)
        lightsON = false;
    }


    if (lightsON){       //skru på

          digitalWrite(rled, LOW); //Debug led blir rød
          digitalWrite(breaklightout, HIGH); //Bremselyset lyser
    }
    else{      //skru av

          digitalWrite(rled, HIGH); //Debug led blir rød
          digitalWrite(breaklightout, LOW); //Bremselyset lyser
    }


  }

}
