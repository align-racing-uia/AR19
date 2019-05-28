#ifndef initialConditions
#define initialConditions

#include "Arduino.h"


class InitialConditions
{
    public: 
    InitialConditions();

    //Initiantl values
    const uint8_t _defaultSystemState = 1;

    //Hall Sensor
    const uint32_t _maxIncreaseHall = 0xFFFFFFFF;
    const uint32_t _maxValueHall = 0xFFFFFFFF;
    const uint16_t _maxTimeDelayHallMillis = 0xFFFF;

    //Potensiometer
    const uint8_t _maxValuePot = 0xFF;
    const uint8_t _minValuePot = 0;
    const uint8_t _errorPot = 12;

    //SensorButton
    const uint8_t _valueTrueBtn = 0xF0;
    const uint8_t _valueFalseBtn = 0x0F;
    const uint8_t _valueFailBtn = 0xAA;

    //StateGuardian
    const uint8_t _lowerThresholdPot = 0x01;
    const uint8_t _lowerThresholdGasPedal = 0x0F;
    const uint32_t _lowerThresholdWheelSpeed = 0xFFFFFFFF;
    const uint32_t _safeEngineSpeed = 0xFFFFFFFF;
    const uint32_t _safeGasPedal = 0xFFFFFFFF;
    const uint32_t _lowerThresholdEngineSpeed = 100;

    //CANbus Reader
    const uint32_t _maxNumberOfByteInCANMessage = 4;
    const uint16_t _canID[20] = {0x019, 0x220, 0x210, 0x2D0, 0x2E0, 0x2F0, 0x235, 0x010, 0x250, 0x2C8, 0x2B0, 0x448, 0x020};


    //CANbus Sender
    const uint16_t _canIdCommunicationCluch = 0x280;
    const uint16_t _canIdCommunicationETC = 0x290;
    const uint16_t _canIdSystemState = 0x2A0;
    const uint16_t _canIdErrorMessages= 0x440;
    const uint16_t _canIdOk = 0x015;

    const uint8_t _canMessageRequestControl = 0xFA;
    const uint8_t _canMessageRelinquishControl = 0xF0;
    const uint8_t _canMessageAborte = 0xAA;

    const uint16_t _canMessageRequestControlETC = 0xF000;
    const uint16_t _canMessageRelinquishControlETC = 0x0F00;

    const uint8_t _canMessageOk = 0xF0;



    //Main Program flow
    const unsigned long _timeDelayMessagesState15Millis = 10;
    const unsigned long _timeDelayMessagesState30Millis = 10;
    const unsigned long _timeDelayMessagesState40Millis = 10;
    const unsigned long _timeDelayMessagesState90Millis = 10;
    const unsigned long _timeDelayMessagesState91Millis = 10;
    const unsigned long _timeDelayMessagesStateErrorMillis = 10;
    

    //Regualtor PID RPM
    const double _ControllerRPMOutputMax = 1.0f;
    const double _ControllerRPMOutputMin = 0.0f;
    const double _KpPIDRPM = 2.105f;
    const double _KiPIDRPM = 4.890f;
    const double _KdPIDRPM = 0.0f;
    const double _setPointRPM = 3000.0f;

    //Regualtor PID Slip
    const double _ControllerSlipOutputMax = 1.0f;
    const double _ControllerSlipOutputMin = 0.0f;
    const double _KpPIDSlip[6] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
    const double _KiPIDSlip[6] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
    const double _KdPIDSlip[6] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
    double _setPointSlip[11] = {0.0f, 5.42f, 6.58f, 7.74f, 8.90f, 10.06f, 12.61f, 15.32f, 18.02f, 20.72f, 23.43f};

    //Ignition Cutter
    const uint8_t cutterPIN = 16;
    
};

#endif 