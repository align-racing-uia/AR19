#ifndef rotaryswitch
#define rotaryswitch

#include "stdint.h"



uint32_t MinimumBoundryValue();
uint32_t MaximumBoundryValue();
bool positionWithinHysteresis(uint32_t inputData);
uint8_t ActualPotmeterPosition();
void CheckRotarySwitch();
void InitiateRotarySwitchPin();
uint8_t CalculatePotmeterPosition(uint32_t dataRaw);
void CalculateIntervals();



uint8_t return8bitRawPotmeterValue();

#endif
