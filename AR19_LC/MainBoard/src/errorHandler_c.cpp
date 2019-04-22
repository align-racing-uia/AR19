#include "Arduino.h"
#include "errorHandler.h"


ErrorHandler::ErrorHandler(uint16_t componentID) : _componentID(componentID)
{

    //cleaning array
    for (int ii = 0; ii < sizeof(_errorLog)/sizeof(errorMessage); ii++){
        _errorLog[ii].errorID = 0;
        _errorLog[ii].componentID = 0;
    }

    numerOfErrors = 0;
}
    
void ErrorHandler::newError(uint16_t errorID, uint16_t componentID){

    for (int ii = 0; ii < sizeof(_errorLog)/sizeof(errorMessage); ii++){
        if(_errorLog[ii].errorID == errorID && _errorLog[ii].componentID == componentID){
            return void();
        }
    }


        //Creating a new errorMessage
        _errorLog[numerOfErrors].errorID = errorID;
        _errorLog[numerOfErrors].componentID = componentID;

        numerOfErrors++;

}

uint32_t ErrorHandler::getErrorMessages(){

    //Returning the oldest message, and moving all adore messages one spott.
    //If no messages, "alle valeus = 0" is returned 
    

    errorMessage e = _errorLog[0];
    if (e.errorID == 0){
        newError(002, _componentID);
    }

    for (uint16_t ii = 0; ii < numerOfErrors-1; ii++){
        _errorLog[ii]= _errorLog[ii+1];
    }
        
    _errorLog[numerOfErrors].errorID = 0;
    _errorLog[numerOfErrors].componentID = 0;
    numerOfErrors--;
    
    toInt tm;
    tm.message = e;
    return tm.int32; 
        
}
uint16_t ErrorHandler::getNumerOfErrors(){
    return numerOfErrors;
}
