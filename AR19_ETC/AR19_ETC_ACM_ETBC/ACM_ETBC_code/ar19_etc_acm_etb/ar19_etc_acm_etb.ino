/*
Align Racing UiA - Electronic Throttle Control
Bachelor Thesis
University of Agder 2019

Members: Stian Rognhaugen, Sander B. Johannessen, Jorgen Nilsen

Title: Electronic Throttle Body Controller
Description: Code for controlling the ETB

v 1.1
Last Revision Date: 20.05.2019 (Implemented launch control functionality)
*/

//  Include Arduino libraries
#include <Arduino.h>

//  Include external libraries
#include "src/mcp2515/mcp2515.h"
#include "src/pid/PID_v1.h"
#include "src/pwm/PWM.h"

//  Include local libraries
#include "ar19_etc_led_settings.h"
#include "ar19_etc_can.h"
#include "ar19_etc_etbc_constants.h"
#include "ar19_etc_sensor.h"


//Set CPU speed if not defined
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//  Creating instances of classes
Can can;
LedSettings led;
SensorData sensor( &can, &led );
PID etbPid( &pid::input, &pid::output, &pid::setpoint, pid::kp, pid::ki, pid::kd, pid::controllerDirection );

void setup()
{
    //  Set pinMode and write LOW to PWM outputs to ensure a known state at startup
    pinMode( etb::directionPinFwd, OUTPUT );
    pinMode( etb::directionPinRev, OUTPUT );
    digitalWrite( etb::directionPinFwd, LOW );
    digitalWrite( etb::directionPinRev, LOW );
    
    //  Set controller specific variables to class functions
    sensor.setVariablesEtb( tps::sensor1Pin, tps::sensor2Pin, tps::value1Min, tps::value1Idle, tps::value1Max, tps::value2Min, tps::value2Idle, tps::value2Max, canbus::tpsId );
    
    //  Initialise CAN
    SPI.begin();
    can.mcp2515.reset();
    can.mcp2515.setBitrate( CAN_500KBPS );
    can.mcp2515.setNormalMode();

    //  Initialise PWM, functions set bools to true if frequency succesfully set
    InitTimersSafe();
    pwmSuccess1 = SetPinFrequencySafe( etb::directionPinFwd, pwm::frequency );
    pwmSuccess2 = SetPinFrequencySafe( etb::directionPinRev, pwm::frequency );

    //  Initialise PID
    etbPid.SetMode( AUTOMATIC );
    etbPid.SetOutputLimits( pid::outputLimitMin, pid::outputLimitMax );
    etbPid.SetSampleTime( pid::sampleTime_ms );

    //  Intialise enable and disable pins
    pinMode( etb::enable, OUTPUT );
    pinMode( etb::disable, OUTPUT );
    digitalWrite( etb::enable, HIGH );
    digitalWrite( etb::disable, LOW );

    //  If setup is successful, write true to bool
    bootSuccess = true;

    //  Checks to see if controller is set to calibrateMode, or setup is successful
    //  Blinks yellow if in calibration mode, green if successful or red if not
    if ( calibrateMode ) {
        sensor.calibrateSetup();
    } else if ( not calibrateMode && pwmSuccess1 && pwmSuccess2 && bootSuccess ) {
        led.blink( led.green, 5 );
    } else {
        led.blink( led.red, 5 );
    }
}

void loop()
{
    //  Runs in calibrate mode loop if set
    while ( calibrateMode ) {
        sensor.calibrate( tps::sensor1Pin, tps::sensor2Pin );

        //  Checks CAN for disable calibrate mode
        if ( can.mcp2515.readMessage( &msgIn ) == MCP2515::ERROR_OK ) {
            if ( msgIn.can_id == canbus::calibrateModeToggleId && msgIn.data[0] == globalFalse && msgIn.data[0] == 16 ) {
                calibrateMode = false;
                led.ledsSwitch( led.off );
            }
        }
    }

    //  Read incoming CAN messages
    if ( can.mcp2515.readMessage( &msgIn ) == MCP2515::ERROR_OK ) {

        //  Set to limp mode    
        if ( msgIn.can_id == canbus::pedalboxImplausibilityId && msgIn.data[0] != 0 ) {
            limpMode = true;
            can.send( canbus::etbLimpModeId, globalTrue, msgIn.data[0] );
            
        } else if ( msgIn.can_id == canbus::pedalboxImplausibilityId && msgIn.data[0] == 0 ) {
            limpMode = false;
            can.send( canbus::etbLimpModeId, globalFalse, msgIn.data[0] );
        
        //  ACM OK ping, sends message back if pinged
        } else if ( msgIn.can_id == canbus::acmPingId && msgIn.data[0] == globalTrue ) {
            can.send( canbus::acmOkId, globalTrue );

        //  APPS data, reads data sent from APPS and writes to variables for use in PID
        } else if ( msgIn.can_id == canbus::appsId ) {
            canbus::throttleTarget      = msgIn.data[0];
            canbus::accPedalDirection   = msgIn.data[1];

        //  Set and reset Launch Control Mode 
        } else if ( msgIn.can_id == canbus::launchModeRequest ) {
            // Checks first byte of message to reset or set mode
            if ( msgIn.data[0] == globalFalse ) {
                launchControlMode = false;
                canbus::launchModeConfirmedMsgCounter = 0;
                can.send( canbus::launchModeConfirmedId, globalFalse );
            } else if ( msgIn.data[0] == globalTrue ) {
                launchControlMode = true;
                canbus::throttleTargetLaunch = msgIn.data[1];
            }

        //  Blip mode request, Set blipmode to true if requested
        } else if ( msgIn.can_id == canbus::requestBlipId && msgIn.data[0] == 0xF0 ) {
            blipMode = true;
        
        // Calibrate mode request
        } else if ( msgIn.can_id == canbus::calibrateModeToggleId && msgIn.data[0] == globalTrue && msgIn.data[1] == 16 ) {
            calibrateMode = true;
        }
    }

    //  Read, write & send TPS SensorData. Assigns current TPS1 value to variable for PID calculation
    pid::input = sensor.tpsData();

    if ( launchControlMode ) {
        //  Sets PID setpoint to throttle target given by launch control 
        pid::setpoint  = canbus::throttleTargetLaunch;
        //  Sends three messages to confirm ETC in launch control mode 
        if ( canbus::launchModeConfirmedMsgCounter <= 3 ) {
            can.send( canbus::launchModeConfirmedId, globalTrue );
            canbus::launchModeConfirmedMsgCounter++;
        }
    } else {
        //  Sets PID setpoint to throttle target given by acceleration pedal
        pid::setpoint  = canbus::throttleTarget;
    }
        
    //  Calculates PID
    etbPid.Compute();

    //  Feedforward; compensating for limp range
    etb::value1 = analogRead(tps::sensor1Pin);

    if  ( etb::value1 > etb::limp_neg && etb::value1 <= etb::limp ) {
        //  Subtracting limp range compensation
        etb::compensation = constrain( pid::output - etb::limpCompensation, - 250, 250 );

    } else if ( etb::value1 < etb::limp_pos && etb::value1 > etb::limp ) {
        //  Adding limp range compensation
        etb::compensation = constrain( pid::output + etb::limpCompensation, - 250, 250 );
        
    } else {
        //  Bypassing feedforward if TPS is not within limp range
        etb::compensation = pid::output;

    }

    //  Checks which mode controller is set to
    //  Limp home mode
    if ( limpMode ) {
        //  Writes a LOW value to both pins
        digitalWrite( etb::directionPinFwd, LOW );
        digitalWrite( etb::directionPinRev, LOW );
        //  Write LOW to enable pin to ensure that nothing is sent to the ETB
        digitalWrite( etb::enable, LOW );

        blipMode            = false;
        launchControlMode   = false;

    //  Blip mode 
    } else if ( blipMode ) {
        //  Sends confirmation of blip back to electronic gear shift controller
        can.send( canbus::blipConfirmedId, globalTrue );

        const unsigned long blipInterval_ms = 200;
        const unsigned long blipLast_ms = millis();

        while ( millis() < blipLast_ms + blipInterval_ms ) {
            //  Illuminates LED purple to indicate blip
            led.ledsSwitch( led.purple );
            //  Setpoint for PID at blip
            pid::setpoint  = 200;
            //  Calculates PID with new setpoint
            etbPid.Compute();
            //  Write PID output to ETB forward and LOW to reverse direction
            digitalWrite( etb::directionPinRev, LOW );
            pwmWrite( etb::directionPinFwd, pid::output );
        }
        //  Turns off LED after blip
        led.ledsSwitch( led.off );
        //  Sets blipmode to false after blip
        blipMode        = false;

    } else {

        //  Write HIGH to enable pin to ensure the H-bridge is enabled if it has been in a safe state
        digitalWrite( etb::enable, HIGH );

        //  If output is positive write to forward pin, if negative write to reverse pin.
        if ( pid::output >= 0 ) {
            
            digitalWrite( etb::directionPinRev, LOW );
            pwmWrite( etb::directionPinFwd, pid::output );
            
            //  For testing
            //can.send( 0x666, etb::directionPinFwd, pid::setpoint, pid::input, pid::output, etb::compensation );
        
        } else if ( pid::output < 0 ) {
            double pidOutputAbs = abs( pid::output );
            digitalWrite( etb::directionPinFwd, LOW );
            pwmWrite( etb::directionPinRev, pidOutputAbs );

            // For testing
            //can.send( 0x666, etb::directionPinRev, pid::setpoint, pid::input, pidOutputAbs, etb::compensation  );
        } 
       
    }

}
