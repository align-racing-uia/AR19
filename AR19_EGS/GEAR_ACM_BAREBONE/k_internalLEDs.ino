
using namespace leds;
void SetRedLED(bool On)
{  
  if (On)
  {  digitalWrite(red, LOW);  }
  else
  {  digitalWrite(red, HIGH); }
}

void SetGreenLED(bool On)
{  
  if (On)
  {  digitalWrite(green, LOW);  }
  else
  {  digitalWrite(green, HIGH); }
}

void SetBlueLED(bool On)
{  
  if (On)
  {  digitalWrite(blue, LOW);  }
  else
  {  digitalWrite(blue, HIGH); }
}
