
// Viktig at denne fila kjem først i kompileringa(etter GEAR_ACM)
    


namespace global{
    const uint8_t sant = 0xF0;
    const uint8_t tull = 0x0F;
}


namespace gearposition{  // CheckGear -  Declarations
    const uint8_t inputPin = 17;  // Make namespace for CheckGear
    uint8_t sensorInput;
    uint8_t voltage;
    uint8_t currentGear = 99;
    uint8_t newGear = 99;
} 

// Variables for gearing.


namespace gearing{
    long timerGearUp = 0;
    long timerGearDown = 0;
    const uint8_t upPin = 10;
    const uint8_t downPin = 3;
    const uint16_t gearTimer = 100;
    const uint16_t gearTimerNeutral = 20;
    long timerNeutral = 0;
    long timerNeutralTimeout = 0;
    const uint16_t neutralTimeout = 500;
    const uint16_t neutralToFirst = 0;
    const uint16_t upshiftRPMLimit = 4000; //RPM needs to be greater than this number to be able to uphift
    const uint16_t downshiftRPMLimit = 5500; //RPM needs to be lower than this number to be able to downshift
    const uint16_t softStartRPM = 2000;
    const uint8_t cLsActivationTimeLimit = 15; // The time frame in which the driver has to act for activating clutchlessdownshift(cLs)
    
}

namespace neutraltofirst{
    uint16_t engageSpeed = 2000;    // How long it takes to engage clutch.
    uint16_t timeOut = 2500;        //How long until the whole neutral to first action is aborted.
    long timeStamp = 0;  

}

namespace variableclutch
{
    long timeStampDeclining;
    uint16_t clutchSpeed = 500;     //Acts as an input to the clutchout void, the speed is sat for different individual purposes
    uint8_t servoDegree = 90;
}


// Defines the pins for internal LEDs.
namespace leds{
    const uint8_t red = 5;
    const uint8_t green = 6;
    const uint8_t blue = 9;
}



// Blip Variables
namespace blipp{
}




//Clutchpressure

namespace clutchpressure
{              
    uint8_t sensorPin = 16;     
    uint8_t sensorValue = 0;
    uint8_t InBar = 0;
    uint8_t disengagedThreshold = 17;
}


//Active clutch



namespace clutch{
    uint8_t servoPin = 16; // pin servo 
    int16_t engage = 0; // servo position 0 degrees
    int16_t disengage = 90; // servo position 90 degrees
    long timestamp = 0;
    const uint8_t duration = 30;

    
}

//Clutchless downshift

namespace clutchlessdownshift
{
const uint16_t timeOut = 400; /* Sets the length of the allowed clutchlessdownshift duration. NB! The timeout can not exceed 400ms due to the FMEA -  Please remember to also make room for the cLsActivationTimeLimit*/
const uint16_t activationTimeLimit = 100;
const uint16_t brakeActivationPressure = 5;
const uint16_t throttleActivationLimit = 5;


}




// Variables derived from the CAN-Bus
namespace cansignal{
    uint8_t accPedalPos1 = 0;
    uint8_t breakPressure1 = 0;
    uint8_t gearUpSignal = global::tull;
    uint8_t gearDownSignal = global::tull;
    uint8_t blipConfirmed = global::tull;
    uint8_t launchModeConfirmed = global::tull;
    uint8_t lcModeConfirmation = global::tull;
    uint8_t requestBlip = global::tull;
    uint8_t sendingEngineCut = global::tull;
    uint8_t gearPosition = 0;
    uint8_t errorStatesGearACM = 0;
    uint8_t rpm1 = 0;
    uint8_t rpm2 = 0;
    uint16_t engineRPM = 0;
    uint8_t clutchPressure = 0;
    uint8_t neutralSignal = global::tull;
    uint8_t launchControlClutch = global::tull;
    uint32_t speedLeftFrontWheel=0;
    uint32_t speedRightFrontWheel=0; 
    //uint32_t vehicleSpeed = (SpeedLeftFrontWheel+SpeedRightFrontWheel)/2 ;

    }


