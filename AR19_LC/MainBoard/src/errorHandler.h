#ifndef errorHandler
#define errorHandler

#include "Arduino.h"

struct errorMessage
{
    uint16_t errorID;
    uint16_t componentID;
};

union toInt
{
    errorMessage message;
    uint32_t int32;
};




class ErrorHandler
{
    
    
    private:

    uint8_t numerOfErrors;
    uint8_t _componentID;
    
    errorMessage _errorLog[10];

        
    public:

    ErrorHandler(uint16_t);

    void newError(uint16_t errorID, uint16_t componentID);
    
    uint32_t getErrorMessages();   

    uint16_t getNumerOfErrors();
};



#endif 