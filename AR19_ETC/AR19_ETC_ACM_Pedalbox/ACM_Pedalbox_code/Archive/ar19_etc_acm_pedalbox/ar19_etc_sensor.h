/*
Align Racing UiA - Electronic Throttle Control
Bachelor Thesis
University of Agder 2019

Members: Stian Rognhaugen, Sander B. Johannessen, Jorgen Nilsen

Title: Sensor node object
Description: Object for reading and processing sensor data

v 2.0
Last Revision Date: 10.05.2019
*/

#ifndef AR19_ETC_SENSOR_H
#define AR19_ETC_SENSOR_H

//  Include local libraries
#include "ar19_etc_can.h"
#include "ar19_etc_led_settings.h"

class SensorData
{
public:
    SensorData( Can*, LedSettings* );
    void setVariablesPedalbox( uint8_t apps1Pin, uint8_t apps2Pin, uint16_t apps1ValueMin, uint16_t apps1ValueMax, uint16_t apps2ValueMin, uint16_t apps2ValueMax, uint8_t bps1Pin, uint8_t bps2Pin, uint16_t bps1ValueMin, uint16_t bps1ValueMax, uint16_t bps2ValueMin, uint16_t bps2ValueMax, uint16_t appsId, uint16_t bpsId );
    void setVariablesEtb( uint8_t tps1Pin, uint8_t tps2Pin, uint16_t tps1ValueMin, uint16_t tps1ValueIdle, uint16_t tps1ValueMax, uint16_t tps2ValueMin, uint16_t tps2ValueIdle, uint16_t tps2ValueMax, uint16_t tpsId );
    void calibrateSetup();
    void calibrate( uint8_t pin1, uint8_t pin2 );
    uint8_t read( uint8_t pin, uint16_t valueMin, uint16_t valueMax );
    uint8_t direction( uint16_t sensorValue, uint16_t &sensorValueOld );
    uint8_t difference_percent( uint16_t value1, uint16_t value2, uint16_t numberOfValues );
    uint8_t implausibilityOutOfRange( uint8_t pin, uint16_t valueMin, uint16_t valueMax );
    uint8_t implausibilityDifference( uint8_t pin1, uint8_t pin2, uint16_t value1Min, uint16_t value1Max, uint16_t value2Min, uint16_t value2Max );
    uint8_t implausibilityCheck( uint8_t impSensor1, uint8_t impSensor2, uint8_t impDiff, unsigned long &implausibleLast_ms, unsigned long implausibleInterval_ms );
    void appsData();
    void bpsData();
    uint8_t tpsData();
private:
    Can* can_;
    LedSettings* led_;
    unsigned long appsLast_ms_  = 0;
    unsigned long bpsLast_ms_   = 0;
    unsigned long tpsLast_ms_   = 0;
    uint16_t apps1ValueOld_, apps2ValueOld_;
    uint8_t apps1Pin_, apps2Pin_, bps1Pin_, bps2Pin_, tps1Pin_, tps2Pin_;
    uint16_t apps1ValueMin_, apps1ValueMax_, apps2ValueMin_, apps2ValueMax_, bps1ValueMin_, bps1ValueMax_, bps2ValueMin_, bps2ValueMax_;
    uint16_t tps1ValueMin_, tps1ValueIdle_, tps1ValueMax_, tps2ValueMin_, tps2ValueIdle_, tps2ValueMax_;
    uint16_t appsId_, bpsId_, tpsId_;
    uint16_t throttleTarget, throttlePosition;
};

#endif