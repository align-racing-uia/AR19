#include "Arduino.h"
#include "dataInput.h"

union CanEncrypter
{
    uint8_t int8[4];
    uint32_t int32;
};


void dataDistributer(ExternalSource** pES, uint16_t n, CanMessage value, uint16_t componentID, ErrorHandler* EH){
    if (value.canID != 0xFFFFFFFF && value.canID != 0)
    {
        for(uint8_t ii = 0; ii < n; ii++){
      
        if (pES[ii]->getCanID() == value.canID)
        {
            CanEncrypter offSeter, newData;

            offSeter.int32 = value.data;

            for(int kk = 0; kk < 4; kk++)
            {
                newData.int8[kk] = 0;
            }            

            for (int jj = 0; jj < pES[ii]->_dataSize; jj++)
            {    
                newData.int8[jj] = offSeter.int8[jj + pES[ii]->_offSet];
            }

            pES[ii]->newData(newData.int32);

            return void();
        }     
    }
     EH->newError(005,componentID);
        return void();
    }   
}



ExternalSource::ExternalSource(InitialConditions* IC, ErrorHandler* EH, uint8_t offSet, uint8_t dataSize, uint16_t componentID, uint16_t canID) :
    _componentID(componentID), _EH(EH), _IC(IC), _canID(canID), _offSet(offSet), _dataSize(dataSize){}

uint16_t ExternalSource::getComponentID(){
    return _componentID;
}
uint16_t ExternalSource::getCanID(){
    return _canID;
}
 
//LAUNCH BUTTONS
SensorButton::SensorButton(InitialConditions* IC, ErrorHandler* EH, uint8_t offSet, uint16_t componentID, uint16_t canID) : ExternalSource(IC, EH, offSet, 1, componentID, canID){
    _SensorButtonPressed = 0;
}
void SensorButton::newData(uint32_t value){
    _SensorButtonPressed = value;
}
bool SensorButton::verificationData(uint32_t value){

    if (value == _IC->_valueTrueBtn || value == _IC->_valueFalseBtn || value == _IC->_valueFailBtn){
        return true;
    }
    
    _EH->newError(104, _componentID);
    return false;
}
void SensorButton::eraseData(){
    _SensorButtonPressed = 0x00;
}

uint16_t SensorButton::getData(){
    return _SensorButtonPressed;}
uint8_t SensorButton::getDataU8(){
    return _SensorButtonPressed;}
uint16_t SensorButton::getDataU16(){
    return 0;}
uint32_t SensorButton::getDataU32(){
    return 0;}
uint64_t SensorButton::getDataU64(){
    return 0;}
int16_t SensorButton::getData16(){
    return 0;}
int32_t SensorButton::getData32(){
    return 0;}
float SensorButton::getDataF(){
    return 0;}
double SensorButton::getDataD(){
    return 0;}


//
//POTENSIOMETER
SensorPotentiometer::SensorPotentiometer(InitialConditions* IC, ErrorHandler* EH, uint8_t offSet, uint16_t componentID, uint16_t canID) : ExternalSource(IC, EH, offSet, 1, componentID, canID){
    _sensorPotentiometerData = 0; 
}
void SensorPotentiometer::newData(uint32_t value){
    //saving the new data 
    _sensorPotentiometerData = value;
}
bool SensorPotentiometer::verificationData(uint32_t value){

    //Chekking the if the valeu is within the alowed intervall
    if( value > _IC->_maxValuePot){
        _EH->newError(102, _componentID);
        return false;
    }
    else if (value < _IC->_minValuePot){
        _EH->newError(103, _componentID);
        return false;
    }
    
    return true;
}
void SensorPotentiometer::eraseData(){
    _sensorPotentiometerData = 0x00;
}

uint16_t SensorPotentiometer::getData(){
    return _sensorPotentiometerData;}
uint8_t SensorPotentiometer::getDataU8(){
    return _sensorPotentiometerData;}
uint16_t SensorPotentiometer::getDataU16(){
    return 0;}
uint32_t SensorPotentiometer::getDataU32(){
    return 0;}
uint64_t SensorPotentiometer::getDataU64(){
    return 0;}
int16_t SensorPotentiometer::getData16(){
    return 0;}
int32_t SensorPotentiometer::getData32(){
    return 0;}
float SensorPotentiometer::getDataF(){
    return 0;}
double SensorPotentiometer::getDataD(){
    return 0;}

//
//HALL SENSOR 
SensorHall::SensorHall(InitialConditions* IC, ErrorHandler* EH, uint8_t offSet, uint16_t componentID, uint16_t canID) : ExternalSource(IC, EH, offSet, 4, componentID, canID){
    
    for (uint16_t ii = 0; ii < sizeof(_sensorHallData)/sizeof(data); ii++){
        _sensorHallData[ii].value = 0;
        _sensorHallData[ii].time = 0;
    }
}
void SensorHall::newData(uint32_t value){

    //Moving all old datas one stepp up in the array and adds the newest one
    for(int ii = sizeof(_sensorHallData)/sizeof(data)-1; ii > 0; ii--){
        _sensorHallData[ii] = _sensorHallData[ii-1];   
    }
    _sensorHallData[0].time = millis();
    _sensorHallData[0].value = value;
}
bool SensorHall::verificationData(uint32_t value){

    //testing the new value to see if it has changes too rappide
    if ((value - _sensorHallData[0].value)/_sensorHallData[0].value > _IC->_maxIncreaseHall){
        _EH->newError(101, _componentID);
        return false;
    }
    //testing the new value to see if it is too high
    else if (value > _IC->_maxValueHall){
        _EH->newError(102, _componentID);
        return false;
    }
    
    return true;
    
}
void SensorHall::eraseData(){
    for (uint16_t ii = 0; ii < sizeof(_sensorHallData)/sizeof(data); ii++){
        _sensorHallData[ii].value = 0;
        _sensorHallData[ii].time = 0;
    }
}

uint16_t SensorHall::getData(){

    //Returning the newest data
    if (millis() - _sensorHallData[0].time < _IC->_maxTimeDelayHallMillis){
        return _sensorHallData[0].value;
    }
    // returning -1 if the data is too old
    else{
        _EH->newError(105, _componentID);
        return -1;
    }    
}

uint8_t SensorHall::getDataU8(){
    return 0;}
uint16_t SensorHall::getDataU16(){
    return 0;}
uint32_t SensorHall::getDataU32(){

    if (millis() - _sensorHallData[0].time < _IC->_maxTimeDelayHallMillis){
        return _sensorHallData[0].value;
    }
    // returning -1 if the data is too old
    else{
        _EH->newError(105, _componentID);
        return -1;
    }  
}
uint64_t SensorHall::getDataU64(){
    return 0;}
int16_t SensorHall::getData16(){
    return 0;}
int32_t SensorHall::getData32(){
    return 0;}
float SensorHall::getDataF(){
    return 0;}
double SensorHall::getDataD(){
    return 0;}

//
//canBUS reader
CanReader::CanReader(InitialConditions* IC, ErrorHandler* EH, uint16_t componentID, MCP2515* mcp2515) :
_IC(IC), _EH(EH), _componentID(componentID), _mcp2515(mcp2515) 
{
    for (int ii=0; ii < 20; ii++){
        _canIDs[ii] = _IC->_canID[ii];
    }
}


CanMessage CanReader::readMessages(){

    CanMessage message;

    if (_mcp2515->readMessage(&_canMsg) == MCP2515::ERROR_OK) {

        for (uint16_t ii = 0; ii < sizeof(_canIDs)/sizeof(_canIDs[0]); ii++){
            if (_canMsg.can_id == _canIDs[ii]){
                union CanEncrypter r;
                
                if (_canMsg.can_dlc > _IC->_maxNumberOfByteInCANMessage){
                    _EH->newError(003, _componentID);
                    _EH->newError(102, _componentID);
                    message.canID = 0xFFFF;
                    message.data = 0xFFFFFFFF;
                    return message;
                }
                
                for (int jj = 0; jj < _canMsg.can_dlc; jj++){
                    r.int8[jj] = _canMsg.data[jj];
                }
                
                message.canID = _canMsg.can_id;
                message.data = r.int32;
                
             }
        }
    }
    else
    {
        message.canID = 0;
        message.data = 0;
    }
    return message;
}