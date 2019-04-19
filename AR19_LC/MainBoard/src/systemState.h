#ifndef systemState
#define systemState

#include "Arduino.h"
#include "initialConditions.h"
#include "errorHandler.h"
#include "dataInput.h"

class stateGuardian{
    private:
    ExternalSource** _ES;
    InitialConditions* _IC;
    ErrorHandler* _EH;
    uint16_t _componentID;
    uint16_t* _systemState;

    bool launchOn();
    bool modeCheck();
    bool controlCheck();
    bool throttleCheck();
    bool buttonRelease();
    bool launchFailure();
    bool safeCheck1();
    bool safeCheck2();

    public:
    stateGuardian(ExternalSource**, uint16_t*, InitialConditions*, ErrorHandler*, uint16_t);
    bool updateSystemState();

};

#endif