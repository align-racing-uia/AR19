#ifndef rotaryswitch.h
#define rotaryswitch.h

#include "stdint.h"



uint32_t MinimumBoundryValue();
uint32_t MaximumBoundryValue();
bool positionWithinHysteresis(uint32_t inputData);
void SendRotarySwitchData();
void CheckRotarySwitch();
void InitiateRotarySwitchPin();
uint32_t CalculatePotmeterPosition(uint32_t dataRaw);
void CalculateIntervals();

#endif
