/*
Align Racing UiA - Electronic Throttle Control
Bachelor Thesis
University of Agder 2019

Members: Stian Rognhaugen, Sander B. Johannessen, Jorgen Nilsen

Title: Electronic Throttle Body Controller
Description: Code for controlling the ETB

v 1.0
Last Revision Date: 11.05.2019
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
    can.mcp2515.setBitrate( CAN_1000KBPS );
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
        led.blink( led.yellow, 5 );
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
    }

    //  Read incoming CAN messages
    if ( can.mcp2515.readMessage( &msgIn ) == MCP2515::ERROR_OK ) {

        //  Set to safe state        
        if ( msgIn.can_id == canbus::pedalboxImplausibilityId && msgIn.data[0] != 0 ) {
            etb::safeState = true;
            can.send( canbus::etbSafeStateId, globalTrue, msgIn.data[0] );
        } else if ( msgIn.can_id == canbus::pedalboxImplausibilityId && msgIn.data[0] == 0 ) {
            etb::safeState = false;
        
        //  ACM OK ping, sends message back if pinged
        } else if ( msgIn.can_id == canbus::acmPingId && msgIn.data[0] == globalTrue ) {
            can.send( canbus::acmOkId, globalTrue );

        //  APPS data, reads data sent from APPS and writes to variables for use in PID
        } else if ( msgIn.can_id == canbus::appsId ) {
            canbus::throttleTarget      = msgIn.data[0];
            canbus::accPedalDirection   = msgIn.data[1];

        //  Blip mode request, Set blipmode to true if requested
        } else if ( msgIn.can_id == canbus::requestBlipId && msgIn.data[0] == 0xF0 ) {
            blipMode = true;
        }
    }

    //  Read, write & send TPS SensorData. Assigns current TPS1 value to variable for PID calculation
    pid::input = sensor.tpsData();
    //  Sets PID setpoint to throttle target 
    pid::setpoint  = canbus::throttleTarget;
        
    //  Calculates PID
    etbPid.Compute();

    //  Checks which mode controller is set to
    //  Safe State
    if ( etb::safeState ) {
        //  Writes a LOW value to both pins
        digitalWrite( etb::directionPinFwd, LOW );
        digitalWrite( etb::directionPinRev, LOW );
        //  Write LOW to enable pin to ensure that nothing is sent to the ETB
        digitalWrite( etb::enable, LOW );

        blipMode    = false;
        lcMode      = false;

    //  Blip mode 
    } else if ( blipMode && not etb::safeState ) {
        //  Sends confirmation of blip back to electronic gear shift controller
        can.send( canbus::blipConfirmedId, globalTrue );

        const unsigned long blipInterval_ms = 200;
        const unsigned long blipLast_ms = millis();
        
        digitalWrite( etb::directionPinRev, LOW );

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
        //  Sets PID setpoint to throttle target
        pid::setpoint   = canbus::throttleTarget;
        //  Sets blipmode to false after blip
        blipMode        = false;

    //  Launch control mode
    } else if ( lcMode && not etb::safeState ) {
        //  To be implemented

    //  Normal mode
    } else {
        //  Write HIGH to enable pin to ensure the H-bridge is enabled if it has been in a safe state
        digitalWrite( etb::enable, HIGH );

        if ( canbus::throttleTarget < 5 ) {
          digitalWrite( etb::directionPinFwd, LOW );
          digitalWrite( etb::directionPinRev, LOW );
          
        //  If output is positive write to forward pin, if negative write to reverse pin.
        } else if ( pid::output >= 0 ) {
            digitalWrite( etb::directionPinRev, LOW );
            pwmWrite( etb::directionPinFwd, pid::output );
            
            //  For testing
            can.send( 0x666, etb::directionPinFwd, pid::setpoint, pid::input, pid::output );
        
        } else if ( pid::output < 0 ) {
            double pidOutputAbs = abs( pid::output );
            digitalWrite( etb::directionPinFwd, LOW );
            pwmWrite( etb::directionPinRev, pidOutputAbs );

            // For testing
            can.send( 0x666, etb::directionPinRev, pid::setpoint, pid::input, pidOutputAbs  );
        } 
       
    }

}
