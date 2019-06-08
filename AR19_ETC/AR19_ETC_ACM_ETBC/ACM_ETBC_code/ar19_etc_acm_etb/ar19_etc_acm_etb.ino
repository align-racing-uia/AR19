/*
Align Racing UiA - Electronic Throttle Control
Bachelor Thesis
University of Agder 2019

Members: Stian Rognhaugen, Sander B. Johannessen, Jorgen Nilsen

Title: Electronic Throttle Body Controller Bench test
Description: Code for controlling the ETB

v 2.0
Last Revision Date: 03.06.2019 (Moved constant to constants header. Added timeout check for reaching limp position)
*/

//  Include Arduino libraries
#include <Arduino.h>

//  Include external libraries
#include "src/mcp2515/mcp2515.h"
#include "src/pid/PID_v1.h"
#include "src/pwm/PWM.h"
#include "math.h"

//  Include local libraries
#include "ar19_etc_acm_etb_constants.h"
#include "ar19_etc_led_settings.h"
#include "ar19_etc_can.h"
#include "ar19_etc_sensor.h"

//Set CPU speed if not defined
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

Can can;
LedSettings led;
SensorData sensor( &can, &led );
PID etbPid( &pidEtbInput, &pidEtbOutput, &pidEtbSetpoint, pidEtbKp, pidEtbKi, pidEtbKd, DIRECT );
PID idlePid( &pidIdleInput, &pidIdleOutput, &pidIdleSetpoint, pidIdleKp, pidIdleKi, pidIdleKd, DIRECT );

struct can_frame msgIn;

void setup()
{
    //  Initialise PWM, functions set bools to true if frequency succesfully set
    InitTimersSafe();
    pwmSuccess1 = SetPinFrequencySafe( etbcOut1Pin, pwmFrequency );
    pwmSuccess2 = SetPinFrequencySafe( etbcOut2Pin, pwmFrequency );
    
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

    //  Initialise PID
    etbPid.SetMode( AUTOMATIC );
    etbPid.SetOutputLimits( pidEtbOutputLimitMin, pidEtbOutputLimitMax );
    etbPid.SetSampleTime( pidEtbSampleTime_us );
    idlePid.SetMode( AUTOMATIC );
    idlePid.SetOutputLimits( pidIdleOutputLimitMin, pidIdleOutputLimitMax );
    idlePid.SetSampleTime( pidIdleSampleTime_us );

    //  If setup is successful, write true to bool
    bootSuccess = true;

    if ( pwmSuccess1 && pwmSuccess2 && bootSuccess ) {
        led.blink( led.green, 5 );
    }
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

        //  Check for APPS implausibility
        } else if ( msgIn.can_id == canIdPedalboxImplausible ) {
            if ( msgIn.data[0] != 0 ) {
                limpMode = true;
            } else {
                limpMode = false;
                reachLimpCheckStarted = false;
                can.send( canIdEtbcLimpModeConfirm, globalFalse );
            }

        //  Check for limp mode toggle
        } else if ( msgIn.can_id == canIdLimpModeToggle ) {
            if ( msgIn.data[0] == globalTrue ) {
                limpMode = true;
            } else if ( msgIn.data[0] == globalFalse ) {
                limpMode = false;
                reachLimpCheckStarted = false;
                can.send( canIdEtbcLimpModeConfirm, globalFalse );
            }

        //  Check for ACM ping
        } else if ( msgIn.can_id == canIdAcmCheck ) {
            if ( msgIn.data[0] == globalTrue ) {
                can.send( acmId, globalTrue );
            }

        //  Check for APPS
        } else if ( msgIn.can_id == canIdApps ) {
            apps1Value              = msgIn.data[0];
            apps2Value              = msgIn.data[1];
            appsDifference_percent  = msgIn.data[2];

        //  Check for RPM
        } else if ( msgIn.can_id == canIdRpm ) {
            rpmValue = ((uint16_t)msgIn.data[7] << 8) | msgIn.data[6];
            can.send( 0x5F1, rpmValue/20 );

        //  Check for LC
        } else if ( msgIn.can_id == canIdLcRequest ) {
            if ( msgIn.data[0] == globalTrue ) {
                launchControlMode = true;
                lcTargetValue = msgIn.data[1];
                can.send( canIdLcConfirm, globalTrue );
            } else {
                launchControlMode = false;
                led.ledsSwitch( led.off );
                can.send( canIdLcConfirm, globalFalse );
            }

        //  Check for blip
        } else if ( msgIn.can_id == canIdBlipRequest ) {
            if ( msgIn.data[0] == globalTrue && msgIn.data[1] == globalTrue ) {
                if ( millis() > blipTimestampLastMsg_ms + blipIntervalLimit_ms && not launchControlMode ) {
                    blipMode = true;
                } else {
                    can.send( canIdBlipConfirm, globalFalse );
                }
            }

        //  Check for change idle RPM
        } else if ( msgIn.can_id == canIdChangeIdleRpm ) {
            if ( msgIn.data[0] == globalTrue ) {
                regulatedIdleEnable = true;
                can.send( canIdChangeIdleRpmConfirm, regulatedIdleEnable, rpmIdle/10 );
            } else if ( msgIn.data[0] == globalFalse ) {
                regulatedIdleEnable = false;
                can.send( canIdChangeIdleRpmConfirm, regulatedIdleEnable, rpmIdle/10 );
            }
            if ( msgIn.data[1] == globalTrue) {
                if ( rpmIdle < 2500 ) {
                    rpmIdle = rpmIdle + 100;
                }
                can.send( canIdChangeIdleRpmConfirm, regulatedIdleEnable, rpmIdle/10 );
            } else if ( msgIn.data[1] == globalFalse ) {
                if ( rpmIdle > 700 ) {
                    rpmIdle = rpmIdle - 100;
                }
                can.send( canIdChangeIdleRpmConfirm, regulatedIdleEnable, rpmIdle/10 );
            }

        //  Check for change idle apps1Value
        } else if ( msgIn.can_id == canIdChangeIdleValue ) {
            if ( msgIn.data[0] == globalTrue ) {
                //  Increment idle position
                if ( msgIn.data[1] == globalTrue ) {
                    if ( tps1Idle < tps1Min + 15gffdas0 ) {
                        tps1Idle = tps1Idle + 5;
                        tps2Idle = tps2Idle - 5;
                        can.send( canIdChangeIdleValueConfirm, globalTrue, tps1Idle );
                    } else {
                        can.send( canIdChangeIdleValueConfirm, globalFalse, tps1Idle );
                    }
                //  Decrement idle position
                } else if ( msgIn.data[1] == globalFalse ) {
                    if ( tps1Idle > tps1Min + 10 ) {
                        tps1Idle = tps1Idle - 5;
                        tps2Idle = tps2Idle + 5;
                        can.send( canIdChangeIdleValueConfirm, globalTrue, tps1Idle );
                    } else {
                        can.send( canIdChangeIdleValueConfirm, globalFalse, tps1Idle );
                    }
                }
            }
        }
    }

    //  Loop while in calibration mode
    while ( calibrateMode ) {
        if ( millis() > calibrateTimestampLastMsg_ms + calibrateInterval_ms ) {
            tps1Value = analogRead(tps1Pin);
            tps2Value = analogRead(tps2Pin);

            tps1Out = constrain( tps1Value/4, 0, 255);
            tps2Out = constrain( tps2Value/4, 0, 255);

            can.send( canIdTps, tps1Out, tps2Out );

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
    tps1ImplausibilityOutofRange = sensor.implausibilityOutOfRange( tps1Pin, tps1Min, tps1Max );
    tps2ImplausibilityOutofRange = sensor.implausibilityOutOfRange( tps2Pin, tps2Min, tps2Max ); 
    tpsImplausibilityDifference = sensor.implausibilityDifference( tps1Pin, tps2Pin, tps1Min, tps1Max, tps2Min, tps2Max );
    tpsImplausible = sensor.implausibilityCheck( tps1ImplausibilityOutofRange, tps2ImplausibilityOutofRange, tpsImplausibilityDifference, tpsLastDiffImplausibility_ms, tpsImplausibilityInterval_ms );

    if ( tpsImplausible != 0 && millis() > tpsImplausibilityLastMsg_ms + tpsImplausibilityInterval_ms ) {
        limpMode = true;
        can.send( canIdTpsImplausible, tpsImplausible );
        tpsImplausibilityLastMsg_ms = millis();
    }
*/
    //  Read and send TPS values
        tps1Value = analogRead( tps1Pin );
        tps2Value = analogRead( tps2Pin );
        //  Maps and constrain for CAN
        tps1Out = constrain( map( tps1Value, tps1Min, tps1Max, 0, 255 ), 0, 255);
        tps2Out = constrain( map( tps2Value, tps2Min, tps2Max, 0, 255 ), 0, 255);
        //  Calculate difference between TPS'
        tpsDifference = tps1Out - tps2Out;
        tpsDifference_percent = (abs( tpsDifference ) * 100) / 256;
        //  Send TPS values over CAN
        if ( millis() > tpsTimestampLastMsg_ms + tpsInterval_ms ) {
            can.send( canIdTps, tps1Out, tps2Out, tpsDifference_percent );
            tpsTimestampLastMsg_ms = millis();
        }

    //  Calculate idle PID
        if ( regulatedIdleEnable ) {
            pidIdleSetpoint = rpmIdle;
            pidIdleInput    = rpmValue;
            idlePid.Compute();
            tps1Idle = pidIdleOutput;
            can.send( 0x668, pidIdleOutput );
        }

    //  Calculate ETB PID
        pidEtbInput = tps1Value;
        //  Sets setpoint to APPS value or LC target if in LC mode
        if ( launchControlMode ) {
            pidEtbSetpoint = map( lcTargetValue, 0, 255, tps1Idle, tps1Max );
            //  Illuminates orange LED to indicate LC mode
            led.ledsSwitch( led.orange );
        } else {
            switch( throttleSetpointMap ) {
                case 1  :   {   
                                apps1Remap = constrain( 0.0038*apps1Value*apps1Value + 0.0405*apps1Value, 0, 255 );
                                pidEtbSetpoint = map( apps1Remap, 0, 255, tps1Idle, tps1Max );
                                break;
                            }
                case 2  :   {   
                                apps1Remap = constrain( -0.0000000008718*pow(apps1Value,5) + 0.0000003414336*pow(apps1Value,4) - 0.0000122737276*pow(apps1Value,3) - 0.002204*pow(apps1Value,2) + 0.38503866096*apps1Value, 0, 255 );
                                pidEtbSetpoint = map( apps1Remap, 0, 255, tps1Idle, tps1Max );
                                break;
                            }
                default :   {
                                pidEtbSetpoint = map( apps1Value, 0, 255, tps1Idle, tps1Max );
                                break;
                            }
            }
        }
            
        etbPid.Compute();
    
    //  Limp home mode
    if ( limpMode ) {
        //  Starts timer at first iteration when Limp mode is set
        if ( not reachLimpCheckStarted ) {
            limpModeTimer_ms = millis();
            reachLimpCheckStarted = true;
        }
        //  Sets bool dependant on TPS in limp position or not
        if ( tps1Value < (tps1Limp + tps1Limp*0.1) && tps1Value > (tps1Limp - tps1Limp*0.1) ) {
            limpReachedOk = true;
        } else {
            limpReachedOk = false;
        }
        //  Checks if TPS has reached limp +-10% before timeout
        if ( millis() > limpModeTimer_ms + reachLimpInterval_ms && not limpReachedOk ) {
            can.send( canIdShutdown, acmId );
        }
        //  Writes LOW to both pins
        digitalWrite( etbcOut1Pin, LOW );
        digitalWrite( etbcOut2Pin, LOW );
        //  Sends confirmation of limp mode active
        can.send( canIdEtbcLimpModeConfirm, globalTrue );
        //  Disable blip and LC mode to prevent sudden enabling after exiting limp mode
        blipMode            = false;
        launchControlMode   = false;

    //  Blip mode 
    } else if ( blipMode ) {
        //  Sends confirmation of blip back to electronic gear shift controller
        can.send( canIdBlipConfirm, globalTrue );
        //  Sets current millis to blip timestamp for duration calculation
        const unsigned long blipTimestamp_ms = millis();
        //  Blips while for duration set by blipDuration_ms
        while ( millis() < blipTimestamp_ms + blipDuration_ms ) {
            //  Illuminates LED purple to indicate blip
            led.ledsSwitch( led.purple );
            //  Setpoint for PID at blip
            pidEtbSetpoint  = tps1Max - 50;
            //  Calculates PID with new setpoint
            etbPid.Compute();
            //  Write PID output to ETB forward and LOW to reverse direction
            digitalWrite( etbcOut2Pin, LOW );
            pwmWrite( etbcOut1Pin, pidEtbOutput );
        }
        //  Turns off LED after blip
        led.ledsSwitch( led.off );
        //  Sets blipmode to false after blip
        blipMode        = false;
        blipTimestampLastMsg_ms = millis();

    //  Normal operation
    } else {

        //  Write HIGH to enable pin to ensure the H-bridge is enabled if it has been in a safe state
        digitalWrite(etbcEnablePin, HIGH );

        //  If output is positive write to forward pin, if negative write to reverse pin.
        if ( pidEtbOutput >= 0 ) {
            
            digitalWrite( etbcOut2Pin, LOW );
            pwmWrite( etbcOut1Pin, pidEtbOutput );
            
            //  For testing
            //can.send( 0x666, etbcOut1Pin, pidEtbOutput, pidEtbInput/4, pidEtbSetpoint/4, apps1Value, apps1Remap );
        
        } else if ( pidEtbOutput < 0 ) {
            double pidEtbOutputAbs = abs( pidEtbOutput );

            digitalWrite( etbcOut1Pin, LOW );
            pwmWrite( etbcOut2Pin, pidEtbOutputAbs );

            // For testing
            //can.send( 0x666, etbcOut2Pin, pidEtbOutput, pidEtbInput/4, pidEtbSetpoint/4, apps1Value, apps1Remap );
        } 
    }
}
