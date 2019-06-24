#include "rotaryswitch.h"
#include "stdint.h"
#include "arduino.h"

//any and all functions and/or variables related to the rotary switch

  //Settings
  const int iRotarySwitchPin = A0; //?Add correct pin number
  uint32_t uiRotarySwitchPositionRaw = 0;
  
  const uint32_t maxSupplyVoltageOffset = 10; //% of supply
  const uint32_t signalHysteresisValue = 20; //how much larger or smaller the value must be than the limit before triggering a value change. A mechanical solution would render this fairly unneccesary.
  const uint32_t numberOfIntervals = 8; //?example


  //"""local""" variables
  uint32_t operatingRange = 100 - maxSupplyVoltageOffset;
  uint32_t maxRawSignalValue = 1023 * operatingRange / 100; //default for arduino (?), 1024 alternatives (including 0)
  uint32_t switchBoundries[numberOfIntervals+1];
  uint32_t currentPotmeterPosition = 0; //Based on the intervals defined above
  uint32_t previousPotmeterPosition = 0; //The value sent over CANbus


void InitiateRotarySwitchPin()
{
  pinMode(iRotarySwitchPin, INPUT);
}

void CalculateIntervals()
{
  for (uint32_t i = 0; i < numberOfIntervals+1; i++) //Creates the interval boundries as an array for later reference
  {
    switchBoundries[i] = ( maxRawSignalValue / numberOfIntervals ) * i;
  }
}

 uint32_t MinimumBoundryValue()
 {
  return (switchBoundries[currentPotmeterPosition-1] + signalHysteresisValue);
 }

 uint32_t MaximumBoundryValue()
 {
   return (switchBoundries[currentPotmeterPosition] - signalHysteresisValue);
 }

 bool positionWithinHysteresis(uint32_t inputData)
 {
  if ((inputData > MinimumBoundryValue()) && inputData <  MaximumBoundryValue())
  { return 0; }
  else
  { return 1; }
}

uint32_t CalculatePotmeterPosition(uint32_t dataRaw) //Calculates current position given the raw data (or raw value minus the offset). First, it checks if the potmeter is in the first bracket. If it is, the function returns 1 and the function is complete. If not, won't return anything and check if the potmeter is in bracket 1 OR 2. If this turns out true, we know the sensor is in bracket 2, or else it would have returned a value last run; thus it returns 2 and the function is complete.
{
  for (int i = 1; i < numberOfIntervals+1; i++)
  {
    if (dataRaw < switchBoundries[i])
    {
      return i;
    }
  }
}

void CheckRotarySwitch()
{      
    uiRotarySwitchPositionRaw = analogRead(iRotarySwitchPin);
    currentPotmeterPosition = CalculatePotmeterPosition(uiRotarySwitchPositionRaw);
}

 void SendRotarySwitchData()
 {
  if ( !positionWithinHysteresis(uiRotarySwitchPositionRaw)  )
  {
    //send current position
  //  Serial.print(currentPotmeterPosition); Serial.print(" "); Serial.print(uiRotarySwitchPositionRaw);
    previousPotmeterPosition = currentPotmeterPosition;
  }
  else
  {
    //send previous potmeter position
 //   Serial.print(previousPotmeterPosition); Serial.print(" "); Serial.print(uiRotarySwitchPositionRaw);
  }
 // Serial.print(uiRotarySwitchPositionRaw); Serial.print(" "); Serial.print(previousPotmeterPosition); Serial.print(" "); Serial.print(currentPotmeterPosition);
}
