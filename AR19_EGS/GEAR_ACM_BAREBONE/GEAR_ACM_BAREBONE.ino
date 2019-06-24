#include <SPI.h>        //Communication with MCP canbus controller
#include <mcp2515.h>    //CAN-Bus library
#include <stdint.h>     //library for aduino
#include <arduino.h>    //Standard arduino library.
#include <Servo.h>      //Servo library



MCP2515 mcp2515(7);
struct can_frame myMessage;


//Creates a servo object
Servo servo;
