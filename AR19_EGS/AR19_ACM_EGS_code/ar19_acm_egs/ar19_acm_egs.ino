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

bool gearUp();
bool gearDown();
bool gearNeutral();
uint16_t getClutchPressure();
uint8_t getCurrentGear();

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
            clutchPressure = analogRead( pinClutchPressureSensor ) /4;
            //  Send raw values over CAN at defined calibration ID
            can.send( canIdEgsCalibrate, gearSensorValue, clutchPressure );
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

            if ( shiftTimeout ) {
                //  Sends error message if request is recieved while in timeout
                can.send( canIdEgsFaultFlag, faultCodeShiftTimeout);   

            } else if ( msgIn.data[0] == globalTrue && msgIn.data[1] == globalFalse ) {
                //  Gear up
                bool shiftSuccessful = gearUp();
                //  Check if shift is successful, blinks green if successful upshift
                if ( shiftSuccessful ){
                    led.ledsSwitch( led.green );
                    shiftTimeout = true;
                    shiftTimeoutLast_ms = millis();
                } else {
                    //  Sends error message indicating failure in upshift
                    can.send( canIdEgsFaultFlag, faultCodeShiftUpFailure );
                }

            } else if ( msgIn.data[0] == globalFalse && msgIn.data[1] == globalTrue ) {
                //  Gear down
                bool shiftSuccessful = gearDown();
                //  Check if shift is successful, blinks orange if successful downshift
                if ( shiftSuccessful ){
                    led.ledsSwitch( led.orange );
                    shiftTimeout = true;
                    shiftTimeoutLast_ms = millis();
                } else {
                    //  Sends error message indicating failure in downshift
                    can.send( canIdEgsFaultFlag, faultCodeShiftDownFailure );
                }

            } else {
                can.send( canIdEgsFaultFlag, faultCodeInvalidGearShiftRequest );
            }
        }

        //  Check for neutral request
    }

    //  Sends CAN with current gear and clutch pressure
    if ( millis() > egsOutInterval_ms + egsOutTimestampLastMsg_ms ) {
        currentGear = getCurrentGear();
        clutchPressure = getClutchPressure()/4;

        can.send( canIdEgsOut, currentGear, clutchPressure );
    }

    //  Checks if timeout has passed 
    if ( shiftTimeout ) {
        if ( millis() > shiftTimeoutInterval_ms + shiftTimeoutLast_ms ) {
            shiftTimeout = false;
            led.ledsSwitch( led.off );
        }
    } else if ( currentGear == 0 ) {
        led.ledsSwitch( led.blue );
    } else {
        led.ledsSwitch( led.off );
    }

}

bool gearUp() {
    currentGear = getCurrentGear();

    if ( currentGear < 6 && currentGear > 2 ) {

        shiftUpTimer_ms = millis();

        while ( millis() < shiftUpTimer_ms + shiftUpInterval_ms ) {
            //  To ensure actuator is not driven in both direction simultaneously
            digitalWrite( pinGearDown, LOW );
            //  Enable actuator shift up
            digitalWrite( pinGearUp, HIGH );
        }
        //  Disable actuator after interval has elapsed
        digitalWrite( pinGearUp, LOW );

    } else {
        //  Send error message if not in correct gear
        can.send( canIdEgsFaultFlag, faultCodeShiftUpFromTopGear );
        return false;
    }

    return true;
}

bool gearDown() {
    if ( currentGear > 1 ) {

        shiftDownTimer_ms = millis();

        while ( millis() < shiftDownTimer_ms + shiftDownInterval_ms ) {
            //  To ensure actuator is not driven in both direction simultaneously
            digitalWrite( pinGearUp, LOW );
            //  Enable actuator shift up
            digitalWrite( pinGearDown, HIGH );
        }
        //  Disable actuator after interval has elapsed
        digitalWrite( pinGearDown, LOW );

    } else if ( currentGear == 1 ) {
        gearNeutral();
    } else {
        //  Send error message if not in correct gear
        can.send( canIdEgsFaultFlag, faultCodeShiftDownFromNeutral );
        return false;
    }

    return true;
}

bool gearNeutral() {
    //  to be implemented
}

uint16_t getClutchPressure() {
    uint16_t sensorValue = analogRead( pinClutchPressureSensor );
    return sensorValue;
}

uint8_t getCurrentGear() {
    uint16_t sensorValue = analogRead( pinGearPositionSensor );
    uint8_t calculatedGear;

    if ( sensorValue > firstGearValue - gearMarginValue && sensorValue < firstGearValue + gearMarginValue ) {
        calculatedGear = 1;
    } else if ( sensorValue > neutralGearValue - gearMarginValue && sensorValue < neutralGearValue + gearMarginValue ) {
        calculatedGear = 0;
    } else if ( sensorValue > secondGearValue - gearMarginValue && sensorValue < secondGearValue + gearMarginValue ) {
        calculatedGear = 2;
    } else if ( sensorValue > thirdGearValue - gearMarginValue && sensorValue < thirdGearValue + gearMarginValue ) {
        calculatedGear = 3;
    } else if ( sensorValue > fourthGearValue - gearMarginValue && sensorValue < fourthGearValue + gearMarginValue ) {
        calculatedGear = 4;
    } else if ( sensorValue > fifthGearValue - gearMarginValue && sensorValue < fifthGearValue + gearMarginValue ) {
        calculatedGear = 5;
    } else if ( sensorValue > sixtGearValue - gearMarginValue && sensorValue < sixtGearValue + gearMarginValue ) {
        calculatedGear = 6;
    } else {
        calculatedGear = 7;
        can.send( canIdEgsFaultFlag, faultCodeGearReadFailure );
    }

    return calculatedGear;
}