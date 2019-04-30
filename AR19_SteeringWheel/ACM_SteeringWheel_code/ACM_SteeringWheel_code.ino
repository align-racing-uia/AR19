#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(7);
struct can_frame myMessage;


long timeStampGearUp = 0;
long timeStampGearDown = 0;
int sant = 0xF0;
int tull = 0x0F;
int red = 5;
int green = 6;
int blue = 9;
int GearUpPIN = 18;
int GearDownPIN = 19;
int GearUpSignal = tull;
int GearDownSignal = tull;
int GearUpMeasure = 0;
int GearDownMeasure = 0;
int terkeliknipe = 200;


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


if (GearUpSignal == sant && millis() - timeStampGearUp > 100){
  
  myMessage.can_id = 0x240;  
  myMessage.can_dlc = 2; 
  myMessage.data[0] = GearUpSignal;
  myMessage.data[1] = tull;
  
  mcp2515.sendMessage(&myMessage);
  timeStampGearUp = millis();
 }



if (GearDownSignal == sant && millis() - timeStampGearDown > 100){
  
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
