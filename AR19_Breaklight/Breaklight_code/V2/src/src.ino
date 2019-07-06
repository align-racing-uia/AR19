/*
Title: ACM Brakelight Controller
Description: Main code for brakelight ACM
v 2.0
Last Revision Date: 21.06.2019

Created by Simon Nylund on 05.04.2019
Copyright © 2019 Align Racing UiA. All rights reserved.
*/


//Libraries
#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>
#include "AllSettings.h"
#include "src/pwm/PWM.h"

MCP2515 mcp2515(7);
struct can_frame canMsg;

int breakpressurepercent = 0;
bool lightsON = false;

void setup() {
 SPI.begin();

  //  Initialise PWM, functions set bools to true if frequency succesfully set
  InitTimersSafe();
  SetPinFrequencySafe( tpsOutPin, pwmFrequency );
  
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

    //  Check for TPS CAN message
    } else if ( canMsg.can_id == canIdTps ) {
        //  Write TPS 2 data to variable
        tps2Value = canMsg.data[1];

        //  Output TPS 2 value as PWM to ECU
        pwmWrite( tpsOutPin, tps2Value );
    
    }

    if (breakPressure > pressureThresholdOn) { //Skrur på bremselyset hvis den går forbi bremsethreshold (Vedien 0-255)
        lightsON = true;
    }
    else if (breakPressure < pressureThresholdOff ) { //Skrur på bremselyset hvis den går forbi bremsethreshold (Vedien 0-255)
        lightsON = false;
    }


    if (lightsON == true){       //skru på

          digitalWrite(rled, LOW); //Debug led blir rød
          digitalWrite(breaklightout, HIGH); //Bremselyset lyser
    }
    else{      //skru av

          digitalWrite(rled, HIGH); //Debug led blir rød
          digitalWrite(breaklightout, LOW); //Bremselyset lyser
    }

  }

}
