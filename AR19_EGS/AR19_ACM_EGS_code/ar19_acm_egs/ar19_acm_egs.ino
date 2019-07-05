/*
Align Racing UiA - Electronic Gear Shift

Written by: Sander B. Johannessen
June 2019

Title: Electronic Gear Shift 
Description: Code for controlling AR19 - Solan's gear shift

v 0.1
Last Revision Date: 05.07.2019
*/

//  Include Arduino libraries
#include <Arduino.h>

//  Include external libraries
#include "src/mcp2515/mcp2515.h"
//#include "src/pid/PID_v1.h"
//#include "src/pwm/PWM.h"
#include "math.h"

//  Include local libraries
#include "ar19_acm_egs_constants.h"
#include "ar19_etc_led_settings.h"
#include "ar19_etc_can.h"

//Set CPU speed if not defined
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

Can can;
LedSettings led;

struct can_frame msgIn;

void gearUp();
void gearDown();
void gearNeutral();

void setup()
{
    //  Set pinMode and write LOW to PWM outputs to ensure a known state at startup
    pinMode( etbcOut1Pin, OUTPUT );
    pinMode( etbcOut2Pin, OUTPUT );
    pinMode( etbcEnablePin, OUTPUT );
    pinMode( etbcDisablePin, OUTPUT );
    digitalWrite( etbcOut1Pin, LOW );
    digitalWrite( etbcOut2Pin, LOW );
    digitalWrite( etbcEnablePin, LOW );
    digitalWrite( etbcDisablePin, LOW );
    
    //  Initialise CAN
    SPI.begin();
    can.mcp2515.reset();
    can.mcp2515.setBitrate( CAN_500KBPS );
    can.mcp2515.setNormalMode();

    //  If setup is successful, write true to bool
    bootSuccess = true;

    //  Blink green LED for visual confirmation of boot successful
    if ( bootSuccess ) {
        led.blink( led.green, 5 );
        can.send( acmId, globalTrue );
    }

}

void loop()
{ 
    //  Loop while in calibration mode
    while ( calibrateMode ) {
        if ( millis() > calibrateTimestampLastMsg_ms + calibrateInterval_ms ) {
            calibrateTimestampLastMsg_ms = millis();
        }

        // Check for calibration mode toggle while in calibration mode
        if ( can.mcp2515.readMessage( &msgIn ) == MCP2515::ERROR_OK ) {
            if ( msgIn.can_id == canIdCalibrateToggle ) {
                //  Disable calibration mode
                if ( msgIn.data[0] == globalFalse && msgIn.data[1] == acmId ) {
                    calibrateMode = false;
                    led.ledsSwitch( led.off );
                }
            }
        }
    }

    //  Reads incoming CAN messages
    if ( can.mcp2515.readMessage( &msgIn ) == MCP2515::ERROR_OK ) {

        // Check for calibration mode toggle
        if ( msgIn.can_id == canIdCalibrateToggle ) {
            //  Enable calibration mode 
            if ( msgIn.data[0] == globalTrue && msgIn.data[1] == acmId ) {
                calibrateMode = true;
                led.ledsSwitch( led.yellow );
            } 
        
        //  Check for ACM ping
        } else if ( msgIn.can_id == canIdAcmCheck ) {
            //  Respond to ACM ping 
            if ( msgIn.data[0] == globalTrue ) {
                can.send( acmId, globalTrue );
            }
        }

        //  Check for gear up request

        //  Check for gear down request

        //  Check for neutral request
    }

}

void gearUp(){
    //  implement
}

void gearDown(){
    //  implement
}

void gearNeutral(){
    //  implement
}