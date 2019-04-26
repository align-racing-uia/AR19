////
//// Voltmeter.h
//// Control_V1.0
////
//// Created by Andreas Wæhle on 13/02/18
////  Copyright © 2018 Andreas Wæhle. All rights reserved.
////
//
//#ifndef Voltmeter_h
//#define Voltmeter_h
//
//
//#include "Arduino.h"
//#include "AllSettings.h"
//#include "Pinout.h"
//
//class Voltmeter
//{
//
//
//public:
//  Voltmeter (uint8_t analogPin);
//  void setup ();
//  void setupVolt ();
//  void read ();
//  void readVolt ();
//  float getValueVolt () const;
//  uint16_t getValue () const;
//  uint8_t getValue_uint8 () const;
//  uint16_t getValue_uint16 () const;
//  uint32_t getValue_uint32 () const;
//  int8_t getValue_percentage () const;
//  int8_t getValue_percentage (uint16_t min, uint16_t max) const;
//  float getValue_voltageDivider () const;
//  void calibrate (uint16_t min, uint16_t max);
//
//private:
//  uint8_t pin_;
//  uint32_t value_;
//  float valueVolt_;
//  bool voltmeterReadProperty_ = 0;
//  uint32_t valueVoltageDivider_;
//  uint8_t minValue_ = AnalogReadResolutionSettings::readValueMin;
//  uint32_t maxValue_ = AnalogReadResolutionSettings::readValueMaxResolutionBits;
//  const float voltageTransferVariable_ = VoltageDividerReadSettings::voltageTransferVariable;
//
//};
//
//#endif
