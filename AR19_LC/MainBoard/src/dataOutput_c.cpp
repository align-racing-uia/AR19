#include "Arduino.h"
#include "dataOutput.h"


union CanDecrypter
{
    uint8_t int8[4];
    uint32_t int32;
};

CanSender::CanSender(InitialConditions* IC, ErrorHandler* EH, uint16_t componentID, MCP2515* mcp2515) : 
    _IC(IC), _EH(EH), _componentID(componentID), _mcp2515(mcp2515)
{

}

void CanSender::newMessage(uint32_t senderID, uint8_t dlc, uint32_t messageValue){
    
        struct can_frame canMsg;
        CanDecrypter decrypter;

        canMsg.can_id = senderID;

        if (dlc <= _IC->_maxNumberOfByteInCANMessage){
            canMsg.can_dlc = dlc;
        }
 
        decrypter.int32 = messageValue;

        for(uint16_t ii = 0; ii < canMsg.can_dlc; ii++)
        {
            canMsg.data[ii] = decrypter.int8[ii];
        }   

        _mcp2515->sendMessage(&canMsg);
    
}






IgnitionCutter::IgnitionCutter(InitialConditions* IC, ErrorHandler* EH, uint16_t componentID) :
    _EH(EH), _IC(IC), _componentID(componentID)
{
    pinMode(_IC->cutterPIN, OUTPUT);
    _frequency = 0xFF;
    _counter = 0;
}


void IgnitionCutter::slaughter(){

    if (_counter - _frequency == 0){

        analogWrite(_IC->cutterPIN, 255); //Setting cutter-pin high
        _counter = 0;

    }
    else if (_counter == 1) 
    {
        analogWrite(_IC->cutterPIN, 0); //Setting cutter-pin low
    }
    else
    {
        _counter++; //does nothing

        if(_counter > _frequency){
            
            _EH->newError(005, _componentID);
            _counter = 0;

        }
    }
}
void IgnitionCutter::uppdateFrequency(uint8_t frequency){
    
    if (frequency != 0){
        _frequency = frequency;
    }
    else
    {
        _frequency = -1;
        _EH->newError(103, _componentID);
    }
    
}

