#include <SPI.h>        //Needed for CAN-Bus
#include <mcp2515.h>    //CAN-Bus library
#include <stdint.h>     //library for aduino
#include <arduino.h>    //Standard arduino library.
#include <math.h>    //Standard arduino library for math.

MCP2515 mcp2515(7);
struct can_frame myMessage;