
//
//  Project Yoda Bachelor Project
//  University of Agder - 2018
//  Group: Sigurd Tollevik, Andreas Wæhle, Kenneth Buvik and Audun Nymoen
//
//  File: StatusLED.h
//  Control_V1.0
//  Descrption: Class for status LED
//
//  Created by Andreas Wæhle on 13/02/2018.
//  Copyright © 2018 Andreas Wæhle. All rights reserved.
//

#ifndef StatusLED_h
#define StatusLED_h

#include "Arduino.h"
#include "AllSettings.h"
#include "Pinout.h"


class StatusLED
{

//---- dotStar LED ----//
public:
  StatusLED ();
  void setup ();

  void setLEDColor (uint8_t red, uint8_t green, uint8_t blue);
  void LEDColorRotation ();
  void LEDColorRotation (uint16_t delayTime);

  void setColorConstant (char color);
  void setColorBlink (char color, uint16_t delayTime);



private:
  void cycleRainbowColors_ (float delayTime);

  uint8_t minLEDValueG_ = StatusLEDSettings::minLEDValue;
  uint8_t minLEDValueR_ = StatusLEDSettings::minLEDValue;
  uint8_t minLEDValueB_ = StatusLEDSettings::minLEDValue;

  bool ledState_ = 0;
  uint32_t previousMillis_ = 0;
  uint32_t currentMillis_ = 0;

  float LEDrotatinTime_;
};

#endif
