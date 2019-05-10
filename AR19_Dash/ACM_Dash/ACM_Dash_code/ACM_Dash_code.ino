#include <stdint.h>     //library for aduino
#include <arduino.h>    //Standard arduino library.
#include <SPI.h>
#include <mcp2515.h>


MCP2515 mcp2515(7);
struct can_frame myMessage;


const uint8_t sant = 0xF0;
const uint8_t tull = 0x0F;
const uint8_t redLED = 17;
const uint8_t greenLED = 16;
const uint8_t neutralPin = 18;
const uint8_t terskel = 150;
const uint8_t intergrertRedLED = 5;
const uint8_t intergrertGreenLED = 6;
const uint8_t neutralLED = 3;
uint16_t clutchPin = 19;


long timeStampCanbus = 0;
uint8_t shutdownActive = tull;
uint8_t neutralSignal = tull;
uint8_t clutchOverride = tull;


void setup() {
  
  pinMode(intergrertRedLED, OUTPUT);
  pinMode(intergrertGreenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(neutralLED, OUTPUT);
  pinMode(neutralPin, OUTPUT);
  pinMode(clutchPin, OUTPUT);
  digitalWrite(neutralLED, HIGH);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);

  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS);
  mcp2515.setNormalMode();

}

void loop() {

// Ta imot data

 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) {
  
   if (myMessage.can_id == 0x270)
   {
      shutdownActive = myMessage.data[0]; // Det første bytet med data
   }
   else if (myMessage.can_id ==  0x19 &&  myMessage.data[0] == sant)
   {
      myMessage.can_id = 0x13;  
      myMessage.can_dlc = 1; 
      myMessage.data[0] = sant;
      mcp2515.sendMessage(&myMessage);
   }
  }
 

// Sende data

  if (analogRead(neutralPin) > terskel)
  {
    neutralSignal = sant;
    digitalWrite(intergrertRedLED, LOW);
  }
  else
  {
    digitalWrite(intergrertRedLED, HIGH);
  }


  if (analogRead(clutchPin) > terskel)
  {
    clutchOverride = sant;
    digitalWrite(intergrertGreenLED, LOW);
  }
  else
  {
    digitalWrite(intergrertGreenLED, HIGH);
  }

if (millis()-timeStampCanbus > 100){
  if (neutralSignal == sant || clutchOverride == sant){
  
    myMessage.can_id = 0x42;  
    myMessage.can_dlc = 2; 
    myMessage.data[0] = neutralSignal;
    myMessage.data[1] = clutchOverride;
    mcp2515.sendMessage(&myMessage);
    timeStampCanbus = millis();

  }

}
  


//LEDSTYRING LOKALT
  if (shutdownActive == sant)
  {
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
  }
  else if (shutdownActive == tull)
  {
    digitalWrite(redLED, LOW );
    digitalWrite(greenLED, HIGH);
  }
}
