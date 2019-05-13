/*
Align Racing UiA - Electronic Throttle Control
Bachelor Thesis
University of Agder 2019

Members: Stian Rognhaugen, Sander Bråten Johannessen, Jørgen Nilsen

Title: Sensor node object
Description: Object for reading and processing sensor data

v 1.0
Last Revision Date: 17.04.2019
*/

#ifndef AR19_ETC_CAN_H
#define AR19_ETC_CAN_H

//  Include external libraries
#include "src/mcp2515/mcp2515.h"

class Can {
public:
    Can();
    
    void send( uint16_t canMsgId, uint8_t canMsgData0 );
    void send( uint16_t canMsgId, uint8_t canMsgData0, uint8_t canMsgData1 );
    void send( uint16_t canMsgId, uint8_t canMsgData0, uint8_t canMsgData1, uint8_t canMsgData2 );
    void send( uint16_t canMsgId, uint8_t canMsgData0, uint8_t canMsgData1, uint8_t canMsgData2, uint8_t canMsgData3 );
    void send( uint16_t canMsgId, uint8_t canMsgData0, uint8_t canMsgData1, uint8_t canMsgData2, uint8_t canMsgData3, uint8_t canMsgData4 );
    void send( uint16_t canMsgId, uint8_t canMsgData0, uint8_t canMsgData1, uint8_t canMsgData2, uint8_t canMsgData3, uint8_t canMsgData4, uint8_t canMsgData5 );
    void send( uint16_t canMsgId, uint8_t canMsgData0, uint8_t canMsgData1, uint8_t canMsgData2, uint8_t canMsgData3, uint8_t canMsgData4, uint8_t canMsgData5, uint8_t canMsgData6 );
    void send( uint16_t canMsgId, uint8_t canMsgData0, uint8_t canMsgData1, uint8_t canMsgData2, uint8_t canMsgData3, uint8_t canMsgData4, uint8_t canMsgData5, uint8_t canMsgData6, uint8_t canMsgData7 );

    MCP2515 mcp2515;

private:

};

#endif