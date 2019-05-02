#include <SPI.h>
#include <mcp2515.h>
#include <stdint.h>     //library for aduino
#include <arduino.h>    //Standard arduino library.

MCP2515 mcp2515(7);
struct can_frame myMessage;


unsigned long timeStampGearUp = 0;
unsigned long timeStampGearDown = 0;
const uint8_t sant = 0xF0;
const uint8_t  tull = 0x0F;
const uint8_t  red = 5;
const uint8_t  green = 6;
const uint8_t  blue = 9;
const uint8_t  GearUpPIN = 18;
const uint8_t  GearDownPIN = 19;
int8_t GearUpSignal = tull;
int8_t GearDownSignal = tull;
int16_t GearUpMeasure = 0;
int16_t GearDownMeasure = 0;
const uint8_t terkeliknipe = 200;


void setup() {
pinMode(red, OUTPUT);
pinMode(green, OUTPUT);
pinMode(blue, OUTPUT);
pinMode(GearUpPIN, OUTPUT);
pinMode(GearDownPIN, OUTPUT);


  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS);
  mcp2515.setNormalMode();

}

void loop() {



// Sende data

GearUpMeasure   = analogRead(GearUpPIN);
GearDownMeasure = analogRead(GearDownPIN);


if ( GearUpMeasure > terkeliknipe)
{
  setLEDsREDGREEN(true);
  GearUpSignal = sant;
}
else 
  {
    setLEDsREDGREEN(false);
    GearUpSignal = tull;
   }


if ( GearDownMeasure > terkeliknipe)
{  
  setLEDsBLUE(true);
  GearDownSignal = sant;
}
else 
  {
    setLEDsBLUE(false);
    GearDownSignal = tull;
  }


if (GearUpSignal == sant && millis() - timeStampGearUp > 50){
  
  myMessage.can_id = 0x240;  
  myMessage.can_dlc = 2; 
  myMessage.data[0] = GearUpSignal;
  myMessage.data[1] = tull;
  
  mcp2515.sendMessage(&myMessage);
  timeStampGearUp = millis();
 }



if (GearDownSignal == sant && millis() - timeStampGearDown > 50){
  
  myMessage.can_id = 0x240;  
  myMessage.can_dlc = 2; 
  myMessage.data[0] = tull;
  myMessage.data[1] = GearDownSignal;
  
  mcp2515.sendMessage(&myMessage);
  timeStampGearDown = millis();
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
