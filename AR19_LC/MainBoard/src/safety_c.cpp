#include "Arduino.h"
#include "safety.h"

Safety::Safety(ExternalSource** ES, InitialConditions* IC, ErrorHandler* EH, uint16_t componentID) :
_componentID(componentID), _EH(EH), _IC(IC), _ES(ES)

}


bool Safety::hallSensorsDown(){
    if(signalChech(_ES[0]->getTime(), _IC->_maxTimeDelayHallMillis) && signalChech(_ES[1]->getTime(), _IC->_maxTimeDelayHallMillis)){
        _EH->newError(203, _componentID)
        return true;
    }
    if(signalChech(_ES[2]->getTime(), _IC->_maxTimeDelayHallMillis) && signalChech(_ES[3]->getTime(), _IC->_maxTimeDelayHallMillis)){
        _EH->newError(204, _componentID)
        return true;
    }

    return false;
}

bool Safety::ETCconectionDown(){
    if(signalChech(_ES[9], _IC->_maxTimeDelayETCMillis)){
        _EH.newError(205, _componentID);
        return true;
    }

    return false;
}


bool Safety::EGSconectionDown(){
    if(signalChech(_ES[6], _IC->_maxTimeDelayEGSMillis)){
        _EH.newError(206, _componentID);
        return true;
    }

    return false;
}





bool signalChech(unsigned long sensorTime, unsigned long refTime){
    return millis() - sensorTime > refTime;
}