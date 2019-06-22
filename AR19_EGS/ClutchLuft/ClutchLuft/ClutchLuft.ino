  #include <SPI.h>        //Needed for CAN-Bus
#include <mcp2515.h>    //CAN-Bus library
#include <stdint.h>     //library for aduino
#include <arduino.h>    //Standard arduino library.
#include <Servo.h>      //Servo library



MCP2515 mcp2515(7);
struct can_frame myMessage;

Servo servo;

int servoPin = 10;
const uint8_t sensorPin = 19;     
uint16_t sensorValue = 0;
uint16_t InBar = 0;
int16_t engage = 1900; // servo position 0 degrees
int16_t disengage = 1200; // servo position 90 degrees
long timer = 4000;
void setup() {                
pinMode(sensorPin, INPUT);     
servo.attach(servoPin);
SPI.begin();
mcp2515.reset();
mcp2515.setBitrate(CAN_500KBPS);
mcp2515.setNormalMode();
}

// the loop routine runs over and over again forever:
void loop() {
  delay(100);
  sensorSend();
delay(100);
  sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
  servo.writeMicroseconds(disengage);
  delay(100);
  sensorSend();
  delay(100);
  sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
delay(100);
sensorSend();
servo.writeMicroseconds(engage); // Connects the engine and wheels

}


void sensorSend()
{
  sensorValue = analogRead(sensorPin);
  InBar = map(sensorValue, 103, 921, 0, 250); // Remaps the bit value to conincide with BAR - info in datasheet
  myMessage.can_id = 0x21;  
  myMessage.can_dlc = 1; 
  myMessage.data[0] = InBar;
  mcp2515.sendMessage(&myMessage);
}
