/*
Align Racing UiA - Electronic Throttle Control
Bachelor Thesis
University of Agder 2019

Members: Stian Rognhaugen, Sander B. Johannessen, Jorgen Nilsen

Title: ACM Pedalbox (For bench test. Hard coded)
Description: Main code for the pedalbox ACM sensornode. 
             Reads APPS & BPS and sends the values over CAN. 

v 2.1
Last Revision Date: 09.06.2019 (Added implausibility checks for APPS and BPS. Removed sensors.h)
*/

//  Include Arduino libraries
#include "Arduino.h"

//  Include external libraries
#include "src\mcp2515\mcp2515.h"

//  Include internal libraries
#include "ar19_etc_acm_pedalbox_constants.h"
#include "ar19_etc_can.h"
#include "ar19_etc_led_settings.h"

//  Set CPU speed if not defined
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//  Creating instances of classes
Can can;
LedSettings led;

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

    //  Checks for APPS implausibility
        //  APPS1 Out of range
        if ( apps1Value > apps1Max + 10 ) {
            apps1ImplausibilityOutofRangeMax = true;
            apps1ImplausibilityOutofRangeMin = false;
        } else if ( apps1Value < apps1Min - 10 ) {
            apps1ImplausibilityOutofRangeMin = true;
            apps1ImplausibilityOutofRangeMax = false;
        } else {
            apps1ImplausibilityOutofRangeMax = false;
            apps1ImplausibilityOutofRangeMin = false;
        }

        //  APPS2 Out of range
        if ( apps2Value < apps2Max - 10 ) {
            apps2ImplausibilityOutofRangeMax = true;
            apps2ImplausibilityOutofRangeMin = false;
        } else if ( apps2Value > apps2Min + 10 ) {
            apps2ImplausibilityOutofRangeMin = true;
            apps2ImplausibilityOutofRangeMax = false;
        } else {
            apps2ImplausibilityOutofRangeMax = false;
            apps2ImplausibilityOutofRangeMin = false;
        }

        if ( apps1ImplausibilityOutofRangeMin && apps2ImplausibilityOutofRangeMin ) {
            appsImplausible = faultCodeAppsBothOutOfRangeMin;
        } else if ( apps1ImplausibilityOutofRangeMin && not apps2ImplausibilityOutofRangeMin ) {
            appsImplausible = faultCodeApps1OutOfRangeMin;
        } else if ( not apps1ImplausibilityOutofRangeMin && apps2ImplausibilityOutofRangeMin ) {
            appsImplausible = faultCodeApps2OutOfRangeMin;
        } else if ( apps1ImplausibilityOutofRangeMax && apps2ImplausibilityOutofRangeMax ) {
            appsImplausible = faultCodeAppsBothOutOfRangeMax;
        } else if ( apps1ImplausibilityOutofRangeMax && not apps2ImplausibilityOutofRangeMax ) {
            appsImplausible = faultCodeApps1OutOfRangeMax;
        } else if ( not apps1ImplausibilityOutofRangeMax && apps2ImplausibilityOutofRangeMax ) {
            appsImplausible = faultCodeApps2OutOfRangeMax;
        } else if ( appsDifference_percent >= 10 && not apps1ImplausibilityOutofRangeMin && not apps2ImplausibilityOutofRangeMin && not apps1ImplausibilityOutofRangeMax && not apps2ImplausibilityOutofRangeMax ) {
            appsImplausible = faultCodeAppsDifferenceAboveTen;
        } else {
            appsImplausible = 0;
            appsLastDiffImplausibility_ms = millis();
        }

    //  Checks for BPS implausibility
        //  BPS1 Out of range
        if ( bps1Value > bps1Max + 30 ) {
            bps1ImplausibilityOutofRangeMax = true;
            bps1ImplausibilityOutofRangeMin = false;
        } else if ( bps1Value < bps1Min - 30 ) {
            bps1ImplausibilityOutofRangeMin = true;
            bps1ImplausibilityOutofRangeMax = false;
        } else {
            bps1ImplausibilityOutofRangeMax = false;
            bps1ImplausibilityOutofRangeMin = false;
        }

        //  BPS2 Out of range
        if ( bps2Value < bps2Max - 30 ) {
            bps2ImplausibilityOutofRangeMax = true;
            bps2ImplausibilityOutofRangeMin = false;
        } else if ( bps2Value > bps2Min + 30 ) {
            bps2ImplausibilityOutofRangeMin = true;
            bps2ImplausibilityOutofRangeMax = false;
        } else {
            bps2ImplausibilityOutofRangeMax = false;
            bps2ImplausibilityOutofRangeMin = false;
        }

        if ( bps1ImplausibilityOutofRangeMin && bps2ImplausibilityOutofRangeMin ) {
            bpsImplausible = faultCodeBpsBothOutOfRangeMin;
        } else if ( bps1ImplausibilityOutofRangeMin && not bps2ImplausibilityOutofRangeMin ) {
            bpsImplausible = faultCodeBps1OutOfRangeMin;
        } else if ( not bps1ImplausibilityOutofRangeMin && bps2ImplausibilityOutofRangeMin ) {
            bpsImplausible = faultCodeBps2OutOfRangeMin;
        } else if (bps1ImplausibilityOutofRangeMax && bps2ImplausibilityOutofRangeMax ) {
            bpsImplausible = faultCodeBpsBothOutOfRangeMax;
        } else if ( bps1ImplausibilityOutofRangeMax && not bps2ImplausibilityOutofRangeMax ) {
            bpsImplausible = faultCodeBps1OutOfRangeMax;
        } else if ( not bps1ImplausibilityOutofRangeMax && bps2ImplausibilityOutofRangeMax ) {
            bpsImplausible = faultCodeBps2OutOfRangeMax;
        } else if ( bpsDifference_percent >= 10 && not bps1ImplausibilityOutofRangeMin && not bps2ImplausibilityOutofRangeMin && not bps1ImplausibilityOutofRangeMax && not bps2ImplausibilityOutofRangeMax ) {
            bpsImplausible = faultCodeBpsDifferenceAboveTen;
        } else {
            bpsImplausible = 0;
            bpsLastDiffImplausibility_ms = millis();
        }

    //  Check if implausibility lasts for more than allowed limit for both APPS and BPS
        if ( appsImplausible != 0 || bpsImplausible != 0 ) {
            if ( millis() > appsLastDiffImplausibility_ms + appsImplausibilityInterval_ms ) {
                can.send( canIdPedalboxImplausibility, appsImplausible, bpsImplausible );
                appsLastDiffImplausibility_ms = millis();
            } else if ( millis() > bpsLastDiffImplausibility_ms + bpsImplausibilityInterval_ms ) {
                can.send( canIdPedalboxImplausibility, appsImplausible, bpsImplausible );
                bpsLastDiffImplausibility_ms = millis();
            }
        }

}
