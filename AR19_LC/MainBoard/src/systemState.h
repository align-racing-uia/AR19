#ifndef systemState
#define systemState

#include "Arduino.h"
#include "initialConditions.h"
#include "errorHandler.h"
#include "dataInput.h"
#include "safety.h"


class stateGuardian{
    private:
    ExternalSource** _ES;
    InitialConditions* _IC;
    ErrorHandler* _EH;
    uint16_t _componentID;
    uint8_t* _systemState;
    Safety* _safety;

    bool launchOn();
    bool modeCheck();
    bool controlCheck();
    bool throttleCheck();
    bool buttonRelease();
    bool launchFailure();
    bool safeCheck1();
    bool safeCheck2();
    bool LCorTCmode();

    public:
    stateGuardian(ExternalSource**, uint8_t*, InitialConditions*, ErrorHandler*, uint16_t, Safety*) ;
    bool updateSystemState();

};

#endif