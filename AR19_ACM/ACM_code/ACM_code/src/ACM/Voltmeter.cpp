////
//// Voltmeter.cpp
//// Control_V1.0
////
//// Created by Andreas Wæhle on 13/02/18
////  Copyright © 2018 Andreas Wæhle. All rights reserved.
////
//
//#include "Voltmeter.h"
//
//
//Voltmeter::Voltmeter (uint8_t analogPin)
//{
//  pin_ = analogPin;
//}
//
//
//void Voltmeter::setup ()
//{
//  analogReadAveraging(AnalogReadSettings::averagingTimes);
//}
//
//void Voltmeter::setupVolt ()
//{
//  analogReference(AnalogReadSettings::referenceType);
//  analogReadResolution(AnalogReadSettings::resolutionBits);
//  analogReadAveraging(AnalogReadSettings::averagingTimes);
//}
//
//void Voltmeter::read ()
//{
//  value_ = analogRead(pin_);
//}
//
//void Voltmeter::readVolt ()
//{
//  valueVolt_ = analogRead(pin_) * voltageTransferVariable_ + VoltageDividerReadSettings::voltageDividerCalibrationValue;
//  valueVolt_ < 0 ? 0 : valueVolt_;
//}
//
//float Voltmeter::getValueVolt () const
//{
//    return valueVolt_;
//}
//
//uint16_t Voltmeter::getValue() const
//{
//    return value_;
//}
//
//
//uint8_t Voltmeter::getValue_uint8 () const
//{
//  return map(value_, minValue_, maxValue_, AnalogReadResolutionSettings::readValueMin, AnalogReadResolutionSettings::readValueMax8Bit);
//}
//
//
//uint16_t Voltmeter::getValue_uint16 () const
//{
//  return map(value_, minValue_, maxValue_, AnalogReadResolutionSettings::readValueMin, AnalogReadResolutionSettings::readValueMax16Bit);
//}
//
//
//uint32_t Voltmeter::getValue_uint32 () const
//{
//  return map(value_, minValue_, maxValue_, AnalogReadResolutionSettings::readValueMin, AnalogReadResolutionSettings::readValueMax32Bit);
//}
//
//
//int8_t Voltmeter::getValue_percentage () const
//{
//  return map(value_, minValue_, maxValue_, AnalogReadResolutionSettings::readValueMin, AnalogReadResolutionSettings::readValuePercentageMax);
//}
//
//int8_t Voltmeter::getValue_percentage (uint16_t min, uint16_t max) const
//{
//  int8_t temp = map(value_, min, max, AnalogReadResolutionSettings::readValueMin, AnalogReadResolutionSettings::readValuePercentageMax);
//  return temp < 0 ? 0 : temp;
//}
//
//void Voltmeter::calibrate (uint16_t min, uint16_t max)
//{
//  minValue_ = min;
//  maxValue_ = max;
//}
