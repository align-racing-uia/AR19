
#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsgGear;
struct can_frame canMsgRPM;

MCP2515 mcp2515(7);


const uint8_t canidRPM = 0x2F0; // offsett 0
const uint8_t canidGear = 0x020; // offsett 3

uint16_t valueRPM;
uint8_t valueGear;





void setup(){

    SPI.begin();
    
    mcp2515.reset();
    mcp2515.setBitrate(CAN_1000KBPS);
    mcp2515.setNormalMode();

    canMsgGear.can_id = canidGear;
    canMsgGear.can_dlc = 2;

    canMsgRPM.can_id = canidRPM;
    canMsgRPM.can_dlc = 5;

    valueGear = 1;
    valueRPM = 2000;

}


void loop(){

    acseleration(8000);
    retard(4000);
    acseleration(5000);
    retard(3000);
    acseleration(6000);
    retard(2500);
    acseleration(3500);
    retard(2000);
    acseleration(4000);
    retard(3500);
    
}

void retard(uint16_t setRPM){

    while(valueRPM > setRPM){

    if (valueRPM < 3000 && valueGear == 2){
        gearDown;
    }

    if (valueRPM < 4000 && valueGear == 3){
        gearDown;
    }
    
    if (valueRPM < 4500 && valueGear == 4){
        gearDown;
    }
    
    if (valueRPM < 5500 && valueGear == 5){
        gearDown;
    }
    
    if (valueRPM < 6000 && valueGear == 6){
        gearDown;
    }

    valueRPM--;
    sendGear();
    sendRPM();
    
    delay(2);

    }
}

void acseleration(uint16_t setRPM){

    while(valueRPM < setRPM){

    if (valueRPM > 3000 && valueGear == 1){
        gearUp;
    }

    if (valueRPM > 4000 && valueGear == 2){
        gearUp;
    }
    
    if (valueRPM > 4500 && valueGear == 3){
        gearUp;
    }
    
    if (valueRPM > 5500 && valueGear == 4){
        gearUp;
    }
    
    if (valueRPM > 6000 && valueGear == 5){
        gearUp;
    }

    valueRPM += 2;
    sendGear();
    sendRPM();
    
    delay(1);

    }
}

void gearUp(){

    valueGear++;
    valueRPM = valueRPM * 4 / 5;

}

void gearDown(){

    valueGear--;

}

void sendGear(){
    
    canMsgGear.data[3] = valueGear;
    mcp2515.sendMessage(&canMsgGear);

}

void sendRPM(){
    
    canMsgRPM.data[0] = valueRPM/37;
    mcp2515.sendMessage(&canMsgRPM);

}

