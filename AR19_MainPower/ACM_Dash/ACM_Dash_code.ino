#include <SPI.h>
#include <mcp2515.h>


MCP2515 mcp2515(7);
struct can_frame myMessage;


const uint8_t sant = 0xF0;
const uint8_t tull = 0x0F;
const uint8_t redLED = 16;
const uint8_t greenLED = 17;
const uint8_t neutralPIN = 19;
const uint8_t terskel = 150;
const uint8_t intergrertRedLED = 5;

long timeStampCANBUS = 0;
uint8_t shutdownActive = tull;
uint8_t neutralSignal = tull;

void setup() {

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(intergrertRedLED, OUTPUT);
  pinMode(neutralPIN, INPUT);

  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS);
  mcp2515.setNormalMode();

}

void loop() {

// Ta imot data

 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) {
  
   if (myMessage.can_id ==  0x270)
   {
      shutdownActive = myMessage.data[0]; // Det første bytet med data
      shutdownActive = tull;
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


  neutralSignal = tull;

  if (analogRead(neutralPIN) > terskel)
  {
    neutralSignal = sant;
    digitalWrite(intergrertRedLED, LOW);
  }
  else
  {
    digitalWrite(intergrertRedLED, HIGH);
  }

  if (neutralSignal == sant || millis() - timeStampCANBUS > 100){
  
    myMessage.can_id = 0x230;  
    myMessage.can_dlc = 1; 
    myMessage.data[0] = neutralSignal;
    
    mcp2515.sendMessage(&myMessage);
    timeStampCANBUS = millis();

  }


//LEDSTYRING LOKALT
  if (shutdownActive == sant)
  {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
  }
  else if (shutdownActive == tull)
  {
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
  }
}