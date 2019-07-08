/*
Align Racing UiA - Electronic Gear Shift

Written by: Sander B. Johannessen
Created: 05.07.2019

Title: Electronic Gear Shift 
Description: Code for controlling AR19 - Solan's gear shift

v 0.2
Last Revision Date: 08.07.2019
Revision history:
v 0.1   - Initial commit. Work in progress.    
v 0.2   - Added clutch pressure checks for downshift and shift from 1st to 2nd. 
        - Added shift to neutral functionality.
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
    while ( calibrateMode ) 
    {
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
    if ( can.mcp2515.readMessage( &msgIn ) == MCP2515::ERROR_OK ) 
    {
        // Check for calibration mode toggle
        if ( msgIn.can_id == canIdCalibrateToggle ) 
        {
            //  Enable calibration mode 
            if ( msgIn.data[0] == globalTrue && msgIn.data[1] == acmId ) 
            {
                calibrateMode = true;
                led.ledsSwitch( led.yellow );
            } 
        
        //  Check for ACM ping
        } 
        else if ( msgIn.can_id == canIdAcmCheck ) 
        {
            //  Respond to ACM ping 
            if ( msgIn.data[0] == globalTrue ) 
            {
                can.send( acmId, globalTrue );
            }

        //  Check for gear shift request
        } 
        else if ( msgIn.can_id == canIdRequestGearShift ) 
        {

            if ( shiftTimeout ) 
            {
                //  Sends error message if request is recieved while in timeout
                can.send( canIdEgsFaultFlag, faultCodeShiftTimeout);   

            } 
            else if ( msgIn.data[0] == globalTrue && msgIn.data[1] == globalFalse ) 
            {
                //  Gear up
                bool shiftSuccessful = gearUp();
                //  Check if shift is successful, blinks green if successful upshift
                if ( shiftSuccessful )
                {
                    led.ledsSwitch( led.green );
                    shiftTimeout = true;
                    shiftTimeoutLast_ms = millis();
                } 
                else 
                {
                    //  Sends error message indicating failure in upshift
                    can.send( canIdEgsFaultFlag, faultCodeShiftUpFailure );
                }

            } 
            else if ( msgIn.data[0] == globalFalse && msgIn.data[1] == globalTrue ) 
            {
                //  Gear down
                bool shiftSuccessful = gearDown();
                //  Check if shift is successful, blinks orange if successful downshift
                if ( shiftSuccessful )
                {
                    led.ledsSwitch( led.orange );
                    shiftTimeout = true;
                    shiftTimeoutLast_ms = millis();
                } 
                else 
                {
                    //  Sends error message indicating failure in downshift
                    can.send( canIdEgsFaultFlag, faultCodeShiftDownFailure );
                }
            } 
            else 
            {
                can.send( canIdEgsFaultFlag, faultCodeInvalidGearShiftRequest );
            }
        }
        else if ( msgIn.can_id == canIdRequestNeutral )
        {
            if ( msgIn.data[0] == globalTrue ) {
                //  Shift to neutral
                bool shiftSuccessful = gearNeutral();
                //  Check if shift is successful, blinks purple if successful shift to neutral
                if ( not shiftSuccessful )
                {
                    //  Sends error message indicating failure in shifting to neutral
                    can.send( canIdEgsFaultFlag, faultCodeShiftNeutralFailure );
                }
            } 
            else 
            {
                can.send( canIdEgsFaultFlag, faultCodeInvalidGearShiftRequest );
            }
        }
    }

    //  Sends CAN with current gear and clutch pressure
    if ( millis() > egsOutInterval_ms + egsOutTimestampLastMsg_ms ) 
    {
        uint8_t oldGear = 99;
        if ( currentGear != 7 )
        {
            oldGear = currentGear;
        }
        currentGear = getCurrentGear();
        clutchPressure = getClutchPressure()/4;

        if ( currentGear == 7 )
        {
            can.send( canIdEgsOut, oldGear, clutchPressure );
        } 
        else 
        {
            can.send( canIdEgsOut, currentGear, clutchPressure );
        }
    }

    //  Checks if timeout has passed 
    if ( shiftTimeout ) 
    {
        if ( millis() > shiftTimeoutInterval_ms + shiftTimeoutLast_ms ) 
        {
            shiftTimeout = false;
            led.ledsSwitch( led.off );
        }
    } 
    else if ( currentGear == 0 ) 
    {
        led.ledsSwitch( led.blue );
    } 
    else 
    {
        led.ledsSwitch( led.off );
    }
}

bool gearUp() {
    //  Get current gear
    currentGear = getCurrentGear();

    //  Shift up from 2nd, 3rd, 4th and 5th ( clutchless )
    if ( currentGear < 6 && currentGear >= 2 ) 
    {
        //  Set timestamp for calculating length of actuation
        shiftTimer_ms = millis();
        //  Perfroming shift for interval determined by variable
        while ( millis() < shiftTimer_ms + shiftUpInterval_ms ) 
        {
            //  To ensure actuator is not driven in both direction simultaneously
            digitalWrite( pinGearDown, LOW );
            //  Enable actuator shift up
            digitalWrite( pinGearUp, HIGH );
        }
        //  Disable actuator after interval has elapsed
        digitalWrite( pinGearUp, LOW );
    } 
    else if ( currentGear == 6 ) 
    {
        //  Send error message if not in correct gear
        can.send( canIdEgsFaultFlag, faultCodeShiftUpFromTopGear );
        return false;
    } 
    //  Shift from 1st to 2nd ( use manual clutch )
    else if ( currentGear == 1 ) 
    {
        //  Get clutch pressure
        clutchPressure = getClutchPressure();
        //  Check that clutch pressure is high enough for shift from 1st to 2nd
        if ( clutchPressure > minClutchPressureShift ) 
        {
            //  Set timestamp for calculating length of actuation
            shiftTimer_ms = millis();
            //  Perfroming shift for interval determined by variable
            while ( millis() < shiftTimer_ms + shiftUpInterval_ms ) 
            {
                //  To ensure actuator is not driven in both direction simultaneously
                digitalWrite( pinGearDown, LOW );
                //  Enable actuator shift up
                digitalWrite( pinGearUp, HIGH );
            }
            //  Disable actuator after interval has elapsed
            digitalWrite( pinGearUp, LOW );
        } 
        else 
        {
            //  Send error message if clutch pressure is too low
            can.send( canIdEgsFaultFlag, faultCodeLowClutchPressure );
            return false;
        }
    }
    //  Shift up from neutral ( half step down, use clutch )
    else if ( currentGear == 0 )
    {
        //  Get clutch pressure
        clutchPressure = getClutchPressure();
        //  Check that clutch pressure is high enough for shift from 1st to 2nd
        if ( clutchPressure > minClutchPressureShift ) 
        {
            //  Set timestamp for calculating length of actuation
            shiftTimer_ms = millis();
            //  Perfroming shift for interval determined by variable
            while ( millis() < shiftTimer_ms + shiftNeutralInterval_ms ) 
            {
                //  To ensure actuator is not driven in both direction simultaneously
                digitalWrite( pinGearUp, LOW );
                //  Enable actuator shift up
                digitalWrite( pinGearDown, HIGH );
            }
            //  Disable actuator after interval has elapsed
            digitalWrite( pinGearDown, LOW );
        } 
        else 
        {
            //  Send error message if clutch pressure is too low
            can.send( canIdEgsFaultFlag, faultCodeLowClutchPressure );
            return false;
        }
    }
    //  Check if shift up
    uint8_t newGear = getCurrentGear();
    if ( newGear != currentGear + 1 )
    {
        can.send( canIdEgsFaultFlag, faultCodeShiftUpFailure );
        return false;
    }
    //  Function returns true to indicate a successful shift
    return true;
}

bool gearDown() {
    //  Get current gear and clutch pressure
    currentGear     = getCurrentGear();
    clutchPressure  = getClutchPressure();
    //  Check that clutch presure is high enough for shift
    if ( clutchPressure > minClutchPressureShift ) 
    {
        //  Check gear
        if ( currentGear > 1 && currentGear <= 6 )
        {
            //  Set timestamp for calculating length of actuation
            shiftTimer_ms = millis();
            //  Perfroming shift for interval determined by variable
            while ( millis() < shiftTimer_ms + shiftDownInterval_ms ) 
            {
                //  To ensure actuator is not driven in both direction simultaneously
                digitalWrite( pinGearUp, LOW );
                //  Enable actuator shift up
                digitalWrite( pinGearDown, HIGH );
            }
            //  Disable actuator after interval has elapsed
            digitalWrite( pinGearDown, LOW );
        } 
        //  If in first gear, shift to neutral
        else if ( currentGear == 1 ) 
        {
            gearNeutral();
        } 
        else 
        {
            //  Send error message if not in correct gear
            can.send( canIdEgsFaultFlag, faultCodeShiftDownFromNeutral );
            return false;
        }
    } 
    else 
    {
        //  Send error message if clutch pressure is too low
        can.send( canIdEgsFaultFlag, faultCodeLowClutchPressure );
        return false;
    }
    //  Check if shift down complete
    uint8_t newGear = getCurrentGear();
    if ( newGear != currentGear - 1 )
    {
        can.send( canIdEgsFaultFlag, faultCodeShiftDownFailure );
        return false;
    }
    //  Function returns true to indicate a successful shift
    return true;
}

bool gearNeutral() {
    //  Get current gear
    currentGear     = getCurrentGear();
    //  Get current clutch pressure
    clutchPressure  = getClutchPressure();
    //  Check if clutch pressure is high enough for shift
    if ( clutchPressure > minClutchPressureShift ) {
        if ( currentGear == 1 ) 
        {
            //  Shift half step up
            //  Set timestamp for calculating length of actuation
            shiftTimer_ms = millis();
            //  Perfroming shift for interval determined by variable
            while ( millis() < shiftTimer_ms + shiftNeutralInterval_ms ) 
            {
                //  To ensure actuator is not driven in both direction simultaneously
                digitalWrite( pinGearDown, LOW );
                //  Enable actuator shift up
                digitalWrite( pinGearUp, HIGH );
            }
            //  Disable actuator after interval has elapsed
            digitalWrite( pinGearUp, LOW );
        } 
        else if ( currentGear == 2 ) 
        {
            //  Shift half step down
            //  Set timestamp for calculating length of actuation
            shiftTimer_ms = millis();
            //  Perfroming shift for interval determined by variable
            while ( millis() < shiftTimer_ms + shiftNeutralInterval_ms ) 
            {
                //  To ensure actuator is not driven in both direction simultaneously
                digitalWrite( pinGearUp, LOW );
                //  Enable actuator shift up
                digitalWrite( pinGearDown, HIGH );
            }
            //  Disable actuator after interval has elapsed
            digitalWrite( pinGearDown, LOW );
        } 
        else 
        {
            // do nothing
        }
    }
    //  Check if shift to neutral is complete
    uint8_t newGear = getCurrentGear();
    if ( newGear != 0 )
    {
        can.send( canIdEgsFaultFlag, faultCodeShiftNeutralFailure );
        return false;
    }
    //  Function returns true to indicate a successful shift
    return true;
}

uint16_t getClutchPressure() {
    uint16_t sensorValue = analogRead( pinClutchPressureSensor );
    return sensorValue;
}

uint8_t getCurrentGear() {
    uint16_t sensorValue = analogRead( pinGearPositionSensor );
    uint8_t calculatedGear;

    if ( sensorValue > firstGearValue - gearMarginValue && sensorValue < firstGearValue + gearMarginValue ) 
    {
        calculatedGear = 1;
    } 
    else if ( sensorValue > neutralGearValue - gearMarginValue && sensorValue < neutralGearValue + gearMarginValue ) 
    {
        calculatedGear = 0;
    } 
    else if ( sensorValue > secondGearValue - gearMarginValue && sensorValue < secondGearValue + gearMarginValue ) 
    {
       calculatedGear = 2;
    } 
    else if ( sensorValue > thirdGearValue - gearMarginValue && sensorValue < thirdGearValue + gearMarginValue ) 
    {
        calculatedGear = 3;
    } 
    else if ( sensorValue > fourthGearValue - gearMarginValue && sensorValue < fourthGearValue + gearMarginValue ) 
    {
        calculatedGear = 4;
    } 
    else if ( sensorValue > fifthGearValue - gearMarginValue && sensorValue < fifthGearValue + gearMarginValue ) 
    {
        calculatedGear = 5;
    } 
    else if ( sensorValue > sixtGearValue - gearMarginValue && sensorValue < sixtGearValue + gearMarginValue ) 
    {
        calculatedGear = 6;
    } 
    else 
    {
        calculatedGear = 7;
        can.send( canIdEgsFaultFlag, faultCodeGearReadFailure );
    }

    return calculatedGear;
}
