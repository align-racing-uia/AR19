/*
Align Racing UiA - Electronic Throttle Control
Bachelor Thesis
University of Agder 2019

Members: Stian Rognhaugen, Sander B. Johannessen, Jorgen Nilsen

Title: ACM Pedalbox (For bench test. Hard coded)
Description: Main code for the pedalbox ACM sensornode. 
             Reads APPS & BPS and sends the values over CAN. 

v 2.0
Last Revision Date: 03.06.2019 03.06.2019 (Moved constant to constants header.)
*/

//  Include Arduino libraries
#include "Arduino.h"

//  Include external libraries
#include "src\mcp2515\mcp2515.h"

//  Include internal libraries
#include "ar19_etc_acm_pedalbox_constants.h"
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

struct can_frame msgIn;

void setup()
{
    //  Initialise CAN
    SPI.begin();
    can.mcp2515.reset();
    can.mcp2515.setBitrate( CAN_500KBPS );
    can.mcp2515.setNormalMode();

    pinMode(apps1Pin,INPUT);
    pinMode(apps2Pin,INPUT);

    led.blink( led.green, 5 );
}

void loop()
{
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
            if ( msgIn.data[0] == globalTrue ) {
                can.send( acmId, globalTrue );
            }
        }
    }

    //  Loop while in calibration mode
    while ( calibrateMode ) {
        if ( millis() > calibrateTimestampLastMsg_ms + calibrateInterval_ms ) {
            apps1Value = analogRead(apps1Pin);
            apps2Value = analogRead(apps2Pin);

            apps1Out = apps1Value/4;
            apps2Out = apps2Value/4;

            can.send( canIdApps, apps1Out, apps2Out );

            bps1Value = analogRead(bps1Pin);
            bps2Value = analogRead(bps2Pin);

            bps1Out = bps1Value/4;
            bps2Out = bps2Value/4;

            can.send( canIdBps, bps1Out, bps2Out );

            calibrateTimestampLastMsg_ms = millis();
        }

        // Check for calibration mode toggle
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
    /*
    apps1ImplausibilityOutOfRange = sensor.implausibilityOutOfRange( apps1Pin, apps1Min, apps1Max );
    apps2ImplausibilityOutOfRange = sensor.implausibilityOutOfRange( apps2Pin, apps2Min, apps2Max );
    appsImplausibilityDifference  = sensor.implausibilityDifference( apps1Pin, apps2Pin, apps1Min, apps1Max, apps2Min, apps2Max );
    bps1ImplausibilityOutOfRange  = sensor.implausibilityOutOfRange( bps1Pin, bps1Min, bps1Max );
    bps2ImplausibilityOutOfRange  = sensor.implausibilityOutOfRange( bps2Pin, bps2Min, bps2Max );
    bpsImplausibilityDifference   = sensor.implausibilityDifference( bps1Pin, bps2Pin, bps1Min, bps1Max, bps2Min, bps2Max );
    appsImplausible   = sensor.implausibilityCheck( apps1ImplausibilityOutOfRange, apps2ImplausibilityOutOfRange, appsImplausibilityDifference, appsLastDiffImplausibility_ms, appsImplausibilityInterval_ms );
    bpsImplausible    = sensor.implausibilityCheck( bps1ImplausibilityOutOfRange, bps2ImplausibilityOutOfRange, bpsImplausibilityDifference, bpsLastDiffImplausibility_ms, bpsImplausibilityInterval_ms );

    if ( ( appsImplausible != 0 || bpsImplausible != 0 ) && millis() > pedalboxImplausibilityLastMsg_ms + pedalboxImplausibilityInterval_ms ) {
        can.send( canIdPedalboxImplausibility, appsImplausible, bpsImplausible );
        pedalboxImplausibilityLastMsg_ms = millis();
    }
    */

    //  Reads, maps and sends APPS values over CAN
    if ( millis() > appsTimestampLastMsg_ms + appsInterval_ms ) {
        apps1Value = analogRead(apps1Pin);
        apps2Value = analogRead(apps2Pin);

        apps1Out = constrain( map( apps1Value, apps1Min, apps1Max, 0, 255), 0, 255);
        apps2Out = constrain( map( apps2Value, apps2Min, apps2Max, 0, 255), 0, 255);

        appsDifference = apps1Out - apps2Out;
        appsDifference_percent = (abs( appsDifference ) * 100) / 256;

        can.send( canIdApps, apps1Out, apps2Out, appsDifference_percent );

        appsTimestampLastMsg_ms = millis();
    }

    //  Reads, maps and sends BPS values over CAN
    if ( millis() > bpsTimestampLastMsg_ms + bpsInterval_ms ) {
        bps1Value = analogRead(bps1Pin);
        bps2Value = analogRead(bps2Pin);

        bps1Out = constrain( map( bps1Value, bps1Min, bps1Max, 0, 255), 0, 255);
        bps2Out = constrain( map( bps2Value, bps2Min, bps2Max, 0, 255), 0, 255);

        bpsDifference = apps1Out - apps2Out;
        bpsDifference_percent = (abs( appsDifference ) * 100) / 256;

        can.send( canIdBps, bps1Out, bps2Out, bpsDifference_percent );

        bpsTimestampLastMsg_ms = millis();
    }

    //  For testing
    /*
        //  Reads, encodes and sends RPM data over CAN
        if ( millis() > rpmTimestampLastMsg_ms + rpmInterval_ms ) {
            rpmValue = analogRead(rpmPin) * 5;

            rpmOut1 = (uint8_t)(rpmValue);
            rpmOut2 = (uint8_t)(rpmValue >> 8);

            can.send( canIdRpm, 0, 0, 0, 0, 0, 0, rpmOut1, rpmOut2 );

            rpmTimestampLastMsg_ms = millis();
        }
    */
}
