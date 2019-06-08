/*
Align Racing UiA - Electronic Throttle Control
Bachelor Thesis
University of Agder 2019

Members: Stian Rognhaugen, Sander B. Johannessen, Jorgen Nilsen

Title: ACM Pedalbox
Description: Main code for the pedalbox ACM sensornode. 
             Reads APPS & BPS and sends the values over CAN. 

v 1.0
Last Revision Date: 06.05.2019
*/

//  Include Arduino libraries
#include "Arduino.h"

//  Include external libraries
#include "src\mcp2515\mcp2515.h"

//  Include internal libraries
#include "ar19_etc_pedalbox_constants.h"
#include "ar19_etc_can.h"
#include "ar19_etc_led_settings.h"
#include "ar19_etc_sensor.h"

//  Set CPU speed if not defined
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//  Creating instances of classes
Can can;
LedSettings led;
SensorData sensor( &can, &led );

void setup()
{
    //  Initialise CAN
    SPI.begin();
    can.mcp2515.reset();
    can.mcp2515.setBitrate( CAN_500KBPS );
    can.mcp2515.setNormalMode();

    //  Set controller specific variables to class functions
    sensor.setVariablesPedalbox(    apps::sensor1Pin, apps::sensor2Pin, 
                                    apps::value1Min, apps::value1Max, apps::value2Min, apps::value2Max, 
                                    bps::sensor1Pin, bps::sensor2Pin, 
                                    bps::value1Min, bps::value1Max, bps::value2Min, bps::value2Max,
                                    canbus::appsId, canbus::bpsId );

    //  If setup is successful, write true to bool
    bootSuccess = true;

    pinMode( apps::sensor1Pin, INPUT_PULLUP );
    pinMode( apps::sensor2Pin, INPUT_PULLUP );
    pinMode( bps::sensor1Pin, INPUT_PULLUP );
    pinMode( bps::sensor2Pin, INPUT_PULLUP );
    
    //  Checks to see if controller is set to calibrateMode, or setup is successful
    //  Blinks yellow if in calibration mode, green if successful or red if not
    if ( calibrateMode ) {
        sensor.calibrateSetup();
    } else if ( not calibrateMode && bootSuccess ) {
        led.blink( led.green, 5 );
    } else {
        led.blink( led.red, 5 );
    }
}

void loop()
{
    //  Reads incoming messages
    if ( can.mcp2515.readMessage(&msgIn) == MCP2515::ERROR_OK ) {

        //  ACM OK ping
        if ( msgIn.can_id == canbus::acmPingId && msgIn.data[0] == globalTrue ) {
            can.send( canbus::acmOkId, globalTrue );

        //  Set to calibrate mode
        } else if ( msgIn.can_id == canbus::calibrateModeToggleId ) {
            if ( not calibrateMode ) {
                calibrateMode = true;
            } else {
                calibrateMode = false;
            }
        }
    }
    
    //  Runs in calibrate mode loop if set
    if ( calibrateMode ) {
        sensor.calibrate( apps::sensor1Pin, apps::sensor2Pin );
    } else {
        //  Read, write & send APPS data
        sensor.appsData();
        //  Read, write & send BPS data
        sensor.bpsData();
        
        //  Checks APPS & BPS for implausibility and sends if one are implausible
        apps::implausibilityOutOfRange1 = sensor.implausibilityOutOfRange( apps::sensor1Pin, apps::value1Min, apps::value1Max );
        apps::implausibilityOutOfRange2 = sensor.implausibilityOutOfRange( apps::sensor2Pin, apps::value2Min, apps::value2Max );
        apps::implausibilityDifference  = sensor.implausibilityDifference( apps::sensor1Pin, apps::sensor2Pin, apps::value1Min, apps::value1Max, apps::value2Min, apps::value2Max );
        bps::implausibilityOutOfRange1  = sensor.implausibilityOutOfRange( bps::sensor1Pin, bps::value1Min, bps::value1Max );
        bps::implausibilityOutOfRange2  = sensor.implausibilityOutOfRange( bps::sensor2Pin, bps::value2Min, bps::value2Max );
        bps::implausibilityDifference   = sensor.implausibilityDifference( bps::sensor1Pin, bps::sensor2Pin, bps::value1Min, bps::value1Max, bps::value2Min, bps::value2Max );
        apps::implausible   = sensor.implausibilityCheck( apps::implausibilityOutOfRange1, apps::implausibilityOutOfRange2, apps::implausibilityDifference, apps::implausibleLast_ms, apps::impInterval_ms );
        bps::implausible    = sensor.implausibilityCheck( bps::implausibilityOutOfRange1, bps::implausibilityOutOfRange2, bps::implausibilityDifference, bps::implausibleLast_ms, bps::impInterval_ms );

        if ( ( apps::implausible != 0 || bps::implausible != 0 ) && millis() > canbus::impLast_ms + canbus::impInterval_ms ) {
            can.send( canbus::pedalboxImplausibilityId, apps::implausible, bps::implausible );
            canbus::impLast_ms = millis();
        }
    }
}
