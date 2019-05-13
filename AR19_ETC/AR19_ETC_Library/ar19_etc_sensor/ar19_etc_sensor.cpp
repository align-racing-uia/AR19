//  Include Arduino libraries
#include "Arduino.h"

//  Include local libraries
#include "ar19_etc_sensor.h"
#include "ar19_etc_can.h"
#include "ar19_etc_led_settings.h"

SensorData::SensorData(Can* can, LedSettings* led)
:   can_(can),
    led_(led)
{

}

void SensorData::setVariablesPedalbox( uint8_t apps1Pin, uint8_t apps2Pin, uint16_t apps1ValueMin, uint16_t apps1ValueMax, uint16_t apps2ValueMin, uint16_t apps2ValueMax, uint8_t bps1Pin, uint8_t bps2Pin, uint16_t bps1ValueMin, uint16_t bps1ValueMax, uint16_t bps2ValueMin, uint16_t bps2ValueMax, uint16_t appsId, uint16_t bpsId )
{
    //  Assign values to variables for use in SensorData class functions 
    apps1Pin_                   = apps1Pin;
    apps2Pin_                   = apps2Pin;
    apps1ValueMin_              = apps1ValueMin; 
    apps1ValueMax_              = apps1ValueMax;
    apps2ValueMin_              = apps2ValueMin;
    apps2ValueMax_              = apps2ValueMax; 
    bps1Pin_                    = bps1Pin; 
    bps2Pin_                    = bps2Pin; 
    bps1ValueMin_               = bps1ValueMin;
    bps1ValueMax_               = bps1ValueMax;
    bps2ValueMin_               = bps2ValueMin;
    bps2ValueMax_               = bps2ValueMax;
    appsId_                     = appsId;
    bpsId_                      = bpsId;
}

void SensorData::setVariablesEtb( uint8_t tps1Pin, uint8_t tps2Pin, uint16_t tps1ValueMin, uint16_t tps1ValueIdle, uint16_t tps1ValueMax, uint16_t tps2ValueMin, uint16_t tps2ValueIdle, uint16_t tps2ValueMax, uint16_t tpsId )
{
    //  Assign values to variables for use in SensorData class functions 
    tps1Pin_                    = tps1Pin;
    tps2Pin_                    = tps2Pin;
    tps1ValueMin_               = tps1ValueMin;
    tps1ValueIdle_              = tps1ValueIdle;
    tps1ValueMax_               = tps1ValueMax;
    tps2ValueMin_               = tps2ValueMin;
    tps2ValueIdle_              = tps2ValueIdle;
    tps2ValueMax_               = tps2ValueMax;
    tpsId_                      = tpsId;
}

void SensorData::calibrateSetup()
{
    //  Set data rate for using Arduino's serial terminal
    Serial.begin(115200);
    Serial.println("Setup complete");
    //  Blink yellow LED to indicate the calibration setup has been successful
    led_->blink( led_->yellow, 5 );
}

void SensorData::calibrate( uint8_t pin1, uint8_t pin2 )
{
    //  Turns on yellow LED to signal calibration mode enabled
    led_->ledsSwitch( led_->yellow );
    //  Define CAN ID for calibration message
    uint16_t calibration_id = 0xCA1;
    //  Read value (0 - 1023 ) from analog sensor
    uint16_t value1 = analogRead( pin1 );
    uint16_t value2 = analogRead( pin2 );
    //  Maps value to 1 byte for sending over CAN
    uint8_t value1Map = map(value1, 0, 1023, 0, 255);
    uint8_t value2Map = map(value2, 0, 1023, 0, 255);
    //  Prints read value to serial monitor, requires Serial.begin() in setup()
    Serial.print("Sensor Value 1: "); Serial.print( value1 ); Serial.print("\n");
    Serial.print("Sensor Value 2: "); Serial.print( value2 ); Serial.print("\n");
    //  Sends value on CAN (!!! Resolution lower, mapped to 0-255 !!!)
    can_->send( calibration_id, value1Map, value2Map );
    //  Delay for readability
    delay(100);
}

uint8_t SensorData::read( uint8_t pin, uint16_t valueMin, uint16_t valueMax )
{
    //  For sensors with positive slope
    if ( valueMin <= valueMax ) {
        //  Reading value
        uint16_t initialValue = analogRead( pin );
        //  Constrains output value so that the signal doesn't overflow map
        uint16_t constrainValue = constrain ( initialValue, valueMin, valueMax );
        //  Remapping value to 1 byte (0-255)
        return map( constrainValue, valueMin, valueMax, 0, 255 );

    //  For sensors with negative slope
    } else {
        //  Reading value
        uint16_t initialValue = analogRead( pin );
        //  Constrains output value so that the signal doesn't overflow map
        uint16_t constrainValue = constrain ( initialValue, valueMax, valueMin );
        //  Remapping value to 1 byte (0-255)
        return map( constrainValue, valueMin, valueMax, 0, 255 );
    }
}

uint8_t SensorData::direction( uint16_t sensorValue, uint16_t &sensorValueOld )
{
    //  x assigns sensitivity for reverse movement
    uint8_t x = 2;
    //  For calculation in function
    uint8_t calculatedDirection;
    //  Calculating the difference between current value and old value
    int16_t valueDifference = sensorValue - sensorValueOld;
    //  Assigns direction if values have changed by x values since last
    if ( valueDifference < 0 && sensorValueOld > sensorValue + x ) {
        //  If pedal is going reverse
        calculatedDirection = 0;
    } else {
        //  If pedal is going forward
        calculatedDirection = 1;
    }
    //  Assigning current value to old value for next iteration
    sensorValueOld = sensorValue;
    //  Return the calculated direction
    return calculatedDirection;
}

uint8_t SensorData::difference_percent( uint16_t value1, uint16_t value2, uint16_t numberOfValues )
{
    //  Differrence between sensors
    int16_t difference = value1 - value2;
    //  Absolute value of difference if value2 is greater than value1
    uint16_t absoluteDifference = abs( difference );
    //  Calculate the percentage difference
    uint8_t difference_percent = ( absoluteDifference * 100 ) / numberOfValues;
    //  Returns difference in percent
    return difference_percent; 
}

uint8_t SensorData::implausibilityOutOfRange( uint8_t pin, uint16_t valueMin, uint16_t valueMax )
{
    //  Read sensor value
    uint16_t value = analogRead( pin );
    //  If sensor value is implausible by being greater than max physical value possible return 110
    if ( value > valueMax + 10 ) {
        return 110;
    //  If sensor value is implausible by being smaller than min physical value possible return 90
    } else if ( value < valueMin - 10 ) {
        return 90;
    // Return 0 if sensor value is plausible
    } else {
        return 0;
    }
}

uint8_t SensorData::implausibilityDifference( uint8_t pin1, uint8_t pin2, uint16_t value1Min, uint16_t value1Max, uint16_t value2Min, uint16_t value2Max )
{
    //  Read value from analog sensors
    uint8_t value1 = read( pin1, value1Min, value1Max );
    uint8_t value2 = read( pin2, value2Min, value2Max );
    //  Calculate difference between sensors
    uint8_t difference = difference_percent( value1, value2, 256 );
    //  Return 100 if difference between sensors is greater than 10 %. 
    if ( difference >= 10 ) {
        return 100;
    } else {
        return 0;
    }
}

uint8_t SensorData::implausibilityCheck( uint8_t impSensor1, uint8_t impSensor2, uint8_t impDiff, unsigned long &implausibleLast_ms )
{
    //  Assigns interval to check for implausibility
    const unsigned long implausibleInterval_ms = 100;
    //  Declares variable to return
    uint8_t implausible;
    //  Checks state of implausibility and returns implausible value if not plausible
    if ( impSensor1 == 90 && impSensor2 == 90 ) {
        implausible = 90;
        implausibleLast_ms = millis();
    } else if ( impSensor1 == 90 && impSensor2 != 90 ) {
        implausible = 91;
        implausibleLast_ms = millis();
    } else if ( impSensor1 != 90 && impSensor2 == 90 ) {
        implausible = 92;
        implausibleLast_ms = millis();
    } else if ( impSensor1 == 110 && impSensor2 == 110 ) {
        implausible = 110;
        implausibleLast_ms = millis();
    } else if ( impSensor1 == 110 && impSensor2 != 110 ) {
        implausible = 111;
        implausibleLast_ms = millis();
    } else if ( impSensor1 != 110 && impSensor2 == 110 ) {
        implausible = 112;
        implausibleLast_ms = millis();
    } else if ( impDiff == 100 ) {
        if ( millis() > implausibleLast_ms + implausibleInterval_ms ) {
            implausible = 100;
        }
    } else {
        implausible = 0;
        implausibleLast_ms = millis();
    }
    return implausible;
}

void SensorData::appsData()
{
    //  Assign interval for sending APPS data 
    const unsigned long appsInterval_ms = 10;
    //  Performs action if current millis is greater than last timestamp
    if ( millis() > appsLast_ms_ + appsInterval_ms ) {
        //  Read sensor values and map to 1 byte
        uint8_t apps1Value = read( apps1Pin_, apps1ValueMin_, apps1ValueMax_ );
        uint8_t apps2Value = read( apps2Pin_, apps2ValueMin_, apps2ValueMax_ ); 
        //  Calculate direction
        uint8_t apps1Direction = direction( apps1Value, apps1ValueOld_ );
        uint8_t apps2Direction = direction( apps2Value, apps2ValueOld_ );
        //  Calculate difference between sensor in percent
        uint8_t appsDifference_percent = difference_percent( apps1Value, apps2Value, 256 );
        //  Assign sensor value to old vairable for direction calculation
        apps1ValueOld_ = apps1Value;
        apps2ValueOld_ = apps2Value;
        //  Send message over CAN
        can_->send( appsId_, apps1Value, apps1Direction, apps2Value, apps2Direction, appsDifference_percent );        
        //  Set timestamp to current millis()
        appsLast_ms_ = millis();
    }
}

void SensorData::bpsData()
{
    //  Assign interval for sending BPS data
    const unsigned long bpsInterval_ms = 100;
    //  Performs action if current millis is greater than last timestamp
    if ( millis() > bpsLast_ms_ + bpsInterval_ms ) {
        //  Read sensor values and map to 1 byte
        uint8_t bps1Value = read( bps1Pin_, bps1ValueMin_, bps1ValueMax_ );
        uint8_t bps2Value = read( bps2Pin_, bps2ValueMin_, bps2ValueMax_ );
        //  Calculate difference between sensor in percent
        uint8_t bpsDifference_percent = difference_percent( bps1Value, bps2Value, 256 );
        //  Send message over CAN
        can_->send( bpsId_, bps1Value, bps2Value, bpsDifference_percent );
        //  Set time to current millis()
        bpsLast_ms_ = millis();
    }
}

uint8_t SensorData::tpsData()
{
    //  Assign interval for sending BPS data
    const unsigned long tpsInterval_ms = 100;
    uint8_t tps1Value;
    //  Performs action if current millis is greater than last timestamp
    if ( millis() > tpsLast_ms_ + tpsInterval_ms ) {
        //  Read sensor values and map to 0-255
                tps1Value = read( tps1Pin_, tps1ValueIdle_, tps1ValueMax_ );
        uint8_t tps2Value = read( tps2Pin_, tps2ValueIdle_, tps2ValueMax_ );
        //  Calculate difference in percent
        uint8_t tpsDifference_percent = difference_percent( tps1Value, tps2Value, 256 );
        //  Send message on CAN
        can_->send( tpsId_, tps1Value, tps2Value, tpsDifference_percent );
        //  Set time to current millis() for sending in interval
        tpsLast_ms_ = millis();
    }
    //  Return TPS value for use as input in PID controller
    return tps1Value;
}