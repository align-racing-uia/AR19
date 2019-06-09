#include <SPI.h>
#include <mcp2515.h>
#include <stdint.h>     //library for aduino
#include <arduino.h>    //Standard arduino library.

MCP2515 mcp2515(7);
struct can_frame myMessage;

long timeStampGearPaddles = 0;
const uint8_t sant = 0xF0;
const uint8_t  tull = 0x0F;
const uint8_t  red = 5;
const uint8_t  green = 6;
const uint8_t  blue = 9;
const uint8_t  GearUpPIN = 18;
const uint8_t  GearDownPIN = 19;
uint8_t GearUpSignal = tull;
uint8_t GearDownSignal = tull;
int16_t GearUpMeasure = 0;
int16_t GearDownMeasure = 0;
const uint8_t terkeliknipe = 500;


void setup() {
pinMode(red, OUTPUT);
pinMode(green, OUTPUT);
pinMode(blue, OUTPUT);
pinMode(GearUpPIN, INPUT_PULLUP);
pinMode(GearDownPIN, INPUT_PULLUP);


  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();

  myMessage.can_id = 0x14;  
  myMessage.can_dlc = 1; 
  myMessage.data[0] = sant;
  mcp2515.sendMessage(&myMessage);

}

void loop() {


// Lese CAN messages

  //Responds to pings
  if (myMessage.can_id == 0x19 && bitRead(myMessage.data[0],3) == 1)
  { 
  myMessage.can_id = 0x14;  
  myMessage.can_dlc = 1; 
  myMessage.data[0] = sant;
  mcp2515.sendMessage(&myMessage);
  }


// Sjekke girhendler

GearUpMeasure   = analogRead(GearUpPIN);
GearDownMeasure = analogRead(GearDownPIN);


if ( GearUpMeasure < terkeliknipe)
{
  setLEDsREDGREEN(true);
  GearUpSignal = sant;
}
else 
    {
  setLEDsREDGREEN(false);
  GearUpSignal = tull;
    }


if ( GearDownMeasure < terkeliknipe)
{  
  setLEDsBLUE(true);
  GearDownSignal = sant;
}
else 
  {
    setLEDsBLUE(false);
    GearDownSignal = tull;
  }


if(millis()-timeStampGearPaddles > 30)
{
  myMessage.can_id = 0x240;  
  myMessage.can_dlc = 2; 
  myMessage.data[0] = GearUpSignal;
  myMessage.data[1] = GearDownSignal;
  
  mcp2515.sendMessage(&myMessage);
  timeStampGearPaddles = millis();
}
}






//
void setLEDsREDGREEN(bool On)
{  
  if (On)
  {
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);

  }
  else
  {
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);

  }
}

void setLEDsBLUE(bool Off)
{  
  if (Off)
  {
  digitalWrite(blue, LOW);

  }
  else
  {
  digitalWrite(blue, HIGH);
  }
}
