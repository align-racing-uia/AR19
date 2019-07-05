/*
Align Racing UiA - Electronic Gear Shift

Written by: Sander B. Johannessen
Created: 05.07.2019

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

bool gearUp( uint8_t outputPin );
bool gearDown( uint8_t outputPin );
bool gearNeutral( uint8_t outputPin );
uint16_t checkClutchPressure( uint8_t sensorPin );
uint8_t checkGear( uint8_t sensorPin );

void setup()
{
    //  Set pinModes and writes LOW to output to ensure a stable startup state
    pinMode( pinGearPositionSensor, INPUT_PULLUP );
    pinMode( pinClutchPressureSensor, INPUT_PULLUP );
    pinMode( pinGearUp, OUTPUT );
    pinMode( pinGearDown, OUTPUT );
    digitalWrite( pinGearUp, LOW );
    digitalWrite( pinGearDown, LOW );
    
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
            //  Read voltage from gear sensor (0-1023) and divides by 4 to send over CAN
            gearSensorValue     = analogRead( pinGearPositionSensor ) /4;
            //  Read voltage from clutch pressure sensor (0-1023) and divides by 4 to send over CAN
            clutchPressureValue = analogRead( pinClutchPressureSensor ) /4;
            //  Send raw values over CAN at defined calibration ID
            can.send( canIdEgsCalibrate, gearSensorValue, clutchPressureValue );
            //  Reset timer to send calibrate message at defined interval
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

        //  Check for gear shift request
        } else if ( msgIn.can_id == canIdRequestGearShift ) {
            if ( msgIn.data[0] == globalTrue && msgIn.data[1] == globalFalse ) {
                
                bool shiftSuccessful = gearUp( pinGearUp );

                if ( shiftSuccessful ){
                    led.ledsSwitch( led.green );
                    shiftTimeout = true;
                    shiftTimeoutLast_ms = millis();
                } else {
                    can.send( canIdEgsFaultFlag, faultCodeShiftUpFailure );
                }

            } else if ( msgIn.data[0] == globalFalse && msgIn.data[1] == globalTrue ) {
                
                bool shiftSuccessful = gearDown( pinGearDown );

                if ( shiftSuccessful ){
                    led.ledsSwitch( led.orange );
                    shiftTimeout = true;
                    shiftTimeoutLast_ms = millis();
                } else {
                    can.send( canIdEgsFaultFlag, faultCodeShiftDownFailure );
                }

            } else {
                can.send( canIdEgsFaultFlag, faultCodeInvalidGearShiftRequest );
            }
        }

        //  Check for neutral request
    }

    //  Checks if timeout has passed 
    if ( shiftTimeout ) {
        if ( millis() > shiftTimeoutInterval_ms + shiftTimeoutLast_ms ) {
            shiftTimeout = false;
            led.ledsSwitch( led.off );
        }
    }

}

bool gearUp( uint8_t outputPin ){
    //  implement
}

bool gearDown( uint8_t outputPin ){
    //  implement
}

bool gearNeutral( uint8_t outputPin ){
    //  implement
}

uint16_t checkClutchPressure( uint8_t sensorPin ){
    uint16_t sensorValue = analogRead( sensorPin );
    return sensorValue;
}

uint8_t checkGear( uint8_t sensorPin ){
    uint16_t sensorValue = analogRead( sensorPin );
    uint8_t currentGear;

    if ( sensorValue > firstGearValue - gearMarginValue && sensorValue < firstGearValue + gearMarginValue ) {
        currentGear = 1;
    } else if ( sensorValue > neutralGearValue - gearMarginValue && sensorValue < neutralGearValue + gearMarginValue ) {
        currentGear = 0;
    } else if ( sensorValue > secondGearValue - gearMarginValue && sensorValue < secondGearValue + gearMarginValue ) {
        currentGear = 2;
    } else if ( sensorValue > thirdGearValue - gearMarginValue && sensorValue < thirdGearValue + gearMarginValue ) {
        currentGear = 3;
    } else if ( sensorValue > fourthGearValue - gearMarginValue && sensorValue < fourthGearValue + gearMarginValue ) {
        currentGear = 4;
    } else if ( sensorValue > fifthGearValue - gearMarginValue && sensorValue < fifthGearValue + gearMarginValue ) {
        currentGear = 5;
    } else if ( sensorValue > sixtGearValue - gearMarginValue && sensorValue < sixtGearValue + gearMarginValue ) {
        currentGear = 6;
    }

    return currentGear;
}