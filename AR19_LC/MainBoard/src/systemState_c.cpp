#include "systemState.h"
#include "Arduino.h"

stateGuardian::stateGuardian(ExternalSource** ES, uint8_t* systemStateP, InitialConditions* IC, ErrorHandler* EH, uint16_t componentID) : 
    _ES(ES), _systemState(systemStateP), _IC(IC), _EH(EH), _componentID(componentID)
{
    *_systemState = _IC->_defaultSystemState;
}

bool stateGuardian::updateSystemState(){


    switch (*_systemState){
        case 1:  //Initialization

            if(launchOn()){
                
                if (LCorTCmode()){

                    *_systemState = 10; //LC and TC on
                    return true;
                
                }else{

                    *_systemState = 40; //Only TC on
                    return true;
                
                }
            }
            return false;
            break;

        case 10:  //Pre-launch
            if(modeCheck()){

                *_systemState = 15;
                return true;

            }else if(!launchOn()){

                *_systemState = 1;
                return true;
                
            }else if(!LCorTCmode()){

                *_systemState = 40; //Only TC on
                 return true;           
            }


            return false;
            break;

        case 15:
        
            if(controlCheck()){
                
                *_systemState = 20;
                return true;

            }
            else if (!modeCheck())
            {

                *_systemState = 90;
                return true;
                
            }
            return false;
            break;

        case 20:  //Launch Ready
            if(throttleCheck()){
            
                *_systemState = 30;
                return true;
            }
            else if (!modeCheck() || !controlCheck())
            {
                *_systemState = 90;
                return true;
            }
            return false;
            break;

        case 30:  //Launch Active
            if(buttonRelease()){
            
                *_systemState = 40;
                return true;
            }
            else if (!controlCheck() || launchFailure())
            {
                *_systemState = 90;
                return true;
            }
            return false;
            break;

        case 40:  //Take off
            
            if(!launchOn()){

                *_systemState = 1;
                return true;
                
            }
            return false;
            break;

        case 90: //Safe mode 1/2         
            if(safeCheck1()){
                *_systemState = 91;
                return true;
            }
            return false;
            break;

        case 91: //Safe mode 2/2
            if (safeCheck2()) {
                *_systemState = 01;
                return true;
            }
            return false;            
            break;

        default: //ERROR 
                _EH->newError(003, _componentID);  
                *_systemState = 90;
                return false;
            break;  
    
    }
    return false;
}
 

bool stateGuardian::launchOn(){
    return(_ES[4]->getDataU8() > _IC->_lowerThresholdPot &&
           //_ES[8]->getDataU16() > _IC->_lowerThresholdEngineSpeed
           );
}
bool stateGuardian::modeCheck(){
    return(_ES[3]->getDataU32() < _IC->_lowerThresholdWheelSpeed &&
           _ES[5]->getDataU8() == _IC->_valueTrueBtn &&
           //_ES[8]->getDataU16() > _IC->_lowerThresholdEngineSpeed
           );
}
bool stateGuardian::LCorTCmode(){
    return(_ES[3]->getDataU32() < _IC->_lowerThresholdWheelSpeed);
}

bool stateGuardian::controlCheck(){
    return(_ES[6]->getDataU8() == _IC->_valueTrueBtn &&
           _ES[9]->getDataU8() == _IC->_valueTrueBtn);
}
bool stateGuardian::throttleCheck(){
    return(_ES[7]->getDataU8() >= _IC->_lowerThresholdGasPedal &&
           _ES[5]->getDataU8() == _IC->_valueTrueBtn);
}
bool stateGuardian::launchFailure(){
    return(_ES[5]->getDataU8() == _IC->_valueFailBtn);
}
bool stateGuardian::buttonRelease(){
    return(_ES[5]->getDataU8() == _IC->_valueFalseBtn);
}
bool stateGuardian::safeCheck1(){
    return(_ES[8]->getDataU32() < _IC->_safeEngineSpeed &&
           _ES[7]->getDataU8() < _IC->_safeGasPedal);
}
bool stateGuardian::safeCheck2(){
    return(_ES[9]->getDataU8() == _IC->_valueFalseBtn);
}