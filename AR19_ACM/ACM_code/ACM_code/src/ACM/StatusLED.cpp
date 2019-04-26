//  Project Yoda Bachelor Project
//  University of Agder - 2018
//  Group: Sigurd Tollevik, Andreas Wæhle, Kenneth Buvik and Audun Nymoen
//
//  File: SPI_dotStar.cpp
//  Control_V1.0
//  Descrption: Cpp file for StatusLED.h
//
//  Created by Andreas Wæhle on 13/02/2018.
//  Copyright © 2018 Andreas Wæhle. All rights reserved.
//

#include "StatusLED.h"

//---- dotStar LED ----//

StatusLED::StatusLED ()
{}


void StatusLED::setup ()
{
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(BLED, OUTPUT);

  digitalWrite(RLED, HIGH);
  digitalWrite(BLED, HIGH);
  digitalWrite(GLED, HIGH);

}

void StatusLED::setLEDColor (uint8_t red, uint8_t green, uint8_t blue)
{
  if (green == 255)
  {
    analogWrite(RLED, red);
    digitalWrite(GLED, HIGH);
    analogWrite(BLED, blue);
  }
  if (red == 255)
  {
    analogWrite(GLED, green);
    digitalWrite(RLED, HIGH);
    analogWrite(BLED, blue);
  }
  if (blue == 255)
  {
    analogWrite(GLED, green);
    digitalWrite(BLED, HIGH);
    analogWrite(RLED, red);
  }
  if (blue == 255 && red == 255)
  {
    analogWrite(GLED, green);
    digitalWrite(BLED, HIGH);
    digitalWrite(RLED, HIGH);
  }
  if (blue == 255 && green == 255)
  {
    analogWrite(RLED, red);
    digitalWrite(BLED, HIGH);
    digitalWrite(GLED, HIGH);
  }
  if (blue == 255 && green == 255)
  {
    analogWrite(BLED, blue);
    digitalWrite(RLED, HIGH);
    digitalWrite(GLED, HIGH);
  }
  if (blue == 255 && green == 255 && red ==255)
  {
    digitalWrite(BLED, HIGH);
    digitalWrite(RLED, HIGH);
    digitalWrite(GLED, HIGH);
  }
  else
  {
  analogWrite(RLED, red);
  analogWrite(GLED, green);
  analogWrite(BLED, blue);
  }
}

void StatusLED::LEDColorRotation ()
{
  LEDrotatinTime_ = StatusLEDSettings::colorRotation/6;
  cycleRainbowColors_ (LEDrotatinTime_);
}

void StatusLED::LEDColorRotation (uint16_t delayTime)
{
  LEDrotatinTime_ = delayTime/6;
  cycleRainbowColors_ (LEDrotatinTime_);
}

void StatusLED::setColorConstant (char color)
{
  switch (color)
  {
    case 'r': setLEDColor(StatusLEDSettings::RED_redLED, StatusLEDSettings::RED_greenLED, StatusLEDSettings::RED_blueLED);
      break;
    case 'o': setLEDColor(StatusLEDSettings::ORANGE_redLED, StatusLEDSettings::ORANGE_greenLED, StatusLEDSettings::ORANGE_blueLED);
      break;
    case 'y': setLEDColor(StatusLEDSettings::YELLOW_redLED, StatusLEDSettings::YELLOW_greenLED, StatusLEDSettings::YELLOW_blueLED);
      break;
    case 'g': setLEDColor(StatusLEDSettings::GREEN_redLED, StatusLEDSettings::GREEN_greenLED, StatusLEDSettings::GREEN_blueLED);
      break;
    case 'b': setLEDColor(StatusLEDSettings::BLUE_redLED, StatusLEDSettings::BLUE_greenLED, StatusLEDSettings::BLUE_blueLED);
      break;
    case 'p': setLEDColor(StatusLEDSettings::PURPLE_redLED, StatusLEDSettings::PURPLE_greenLED, StatusLEDSettings::PURPLE_blueLED);
      break;
    case 'w': setLEDColor(StatusLEDSettings::WHITE_redLED, StatusLEDSettings::WHITE_greenLED, StatusLEDSettings::WHITE_blueLED);
      break;
    default: setLEDColor(StatusLEDSettings::minLEDValue, StatusLEDSettings::minLEDValue, StatusLEDSettings::minLEDValue);
      break;
  }
}

void StatusLED::setColorBlink (char color, uint16_t delayTime)
{
  currentMillis_ = millis();
  if (currentMillis_ - previousMillis_ >= delayTime)
  {
    previousMillis_ = currentMillis_;
    if (ledState_ == 0)
    {
      ledState_ = 1;
      setColorConstant (color);
    }
    else
    {
      ledState_ = 0;
      setColorConstant (0);
    }
  }
}

void StatusLED::cycleRainbowColors_ (float delayTime)
{
  setLEDColor(StatusLEDSettings::PURPLE_redLED, StatusLEDSettings::PURPLE_greenLED, StatusLEDSettings::PURPLE_blueLED);
  delay(delayTime);
  setLEDColor(StatusLEDSettings::BLUE_redLED, StatusLEDSettings::BLUE_greenLED, StatusLEDSettings::BLUE_blueLED);
  delay(delayTime);
  setLEDColor(StatusLEDSettings::GREEN_redLED, StatusLEDSettings::GREEN_greenLED, StatusLEDSettings::GREEN_blueLED);
  delay(delayTime);
  setLEDColor(StatusLEDSettings::YELLOW_redLED, StatusLEDSettings::YELLOW_greenLED, StatusLEDSettings::YELLOW_blueLED);
  delay(delayTime);
  setLEDColor(StatusLEDSettings::ORANGE_redLED, StatusLEDSettings::ORANGE_greenLED, StatusLEDSettings::ORANGE_blueLED);
  delay(delayTime);
  setLEDColor(StatusLEDSettings::RED_redLED, StatusLEDSettings::RED_greenLED, StatusLEDSettings::RED_blueLED);
  delay(delayTime);
}
