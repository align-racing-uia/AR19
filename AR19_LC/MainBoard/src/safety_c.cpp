#include "Arduino.h"
#include "safety.h"

Safety::Safety(ExternalSource** ES, InitialConditions* IC, ErrorHandler* EH, uint16_t componentID) :
_componentID(componentID), _EH(EH), _IC(IC), _ES(ES)
{

}


bool Safety::hallSensorsDown(){
    if(hallChech(_ES[0]->getTime()) && hallChech(_ES[1]->getTime())){
        _EH->newError(203, _componentID)
        return true;
    }
    if(hallChech(_ES[2]->getTime()) && hallChech(_ES[3]->getTime())){
        _EH->newError(204, _componentID)
        return true;
    }

    return false;
}








bool hallChech(unsigned long sensorTime){
    return millis() - sensorTime > _IC->_maxTimeDelayHallMillis;
}