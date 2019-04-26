#include <Arduino.h>
#include "Pinout.h"
#include "StatusLED.h"

StatusLED onboardLED = StatusLED ();

void setup()
{
  onboardLED.setup();
  for (uint8_t i = 5; i--;)
  {
    onboardLED.LEDColorRotation(100);
  }
}

void loop()
{
  onboardLED.LEDColorRotation(12000);
}
