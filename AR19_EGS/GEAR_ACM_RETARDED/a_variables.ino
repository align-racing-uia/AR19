
// Viktig at denne fila kjem først i kompileringa(etter GEAR_ACM)
    


namespace global{
    const uint8_t sant = 0xF0;
    const uint8_t tull = 0x0F;
}


namespace gearposition{  // CheckGear -  Declarations
    const uint8_t inputPin = 17;  // Make namespace for CheckGear
    uint8_t sensorInput;
    uint8_t voltage;
    uint8_t currentGear = 0;
    uint8_t newGear = 0;

} 


namespace gotoneutral
{
const uint16_t neutralTimeout = 2000; // Length of a goToNeutral attempt duartion
long gearTimerNeutral = 0;
long shiftTimer = 0;
const uint16_t shiftingPause = 20;
const uint16_t shiftAttmptLength = 80;
const uint16_t stopSpeedLimit = 3; //Speed must be less to let the car release the clutch
}

// Variables for gearing.


namespace gearing{
    long timerGearUp = 0;
    long timerGearDown = 0;
    const uint8_t upPin = 10;
    const uint8_t downPin = 3;
    const uint16_t gearTimer = 550;
    const uint16_t gearTimerTimeout = gearTimer + 20;
    long timerNeutral = 0;
    long timerNeutralTimeout = 0;
    const uint16_t upshiftRPMLimit = 4000; //RPM needs to be greater than this number to be able to uphift
    const uint16_t downshiftRPMLimit = 5500; //RPM needs to be lower than this number to be able to downshift
    const uint16_t softStartRPM = 3000; //Lowest RPM requiered to start the softStart for the vehicle
    const uint8_t cLsActivationTimeLimit = 15; // The time frame in which the driver has to act for activating clutchlessdownshift(cLs)
    const uint16_t downshiftClutchEngageSpeed = 400;
    long timerBlipGearingDelay = 0;
    const uint8_t blipGearingDelay = 70; //The time it takes to 
    
}

namespace neutraltofirst{
    uint16_t engageSpeed = 1500;    // How long it takes to engage clutch.
    uint16_t preEngaging = 500;
    uint16_t timer = engageSpeed + preEngaging;        //How long until the whole neutral to first action is aborted.
    uint16_t timeOut = timer + 20;
    long timeStamp = 0;
}

namespace geartomatchspeed{
    const uint8_t wheelRadius = 8; //wheel radius given in inches
    const uint8_t differentialSprocket = 35; //Number of tooths at rear sprocket gear
    uint8_t clutchInCheck = 0;
    uint16_t firstGearRPMSpeedRatio = 0;
    uint16_t secondGearRPMSpeedRatio = 0;
    uint16_t thirdGearRPMSpeedRatio = 0;
    uint16_t fourthGearRPMSpeedRatio = 0;
    uint16_t fifthGearRPMSpeedRatio = 0;
    uint16_t sixthGearRPMSpeedRatio = 0;

}

namespace variableclutch
{
    long timeStampDeclining;
    uint16_t clutchSpeed;    //Acts as an input to the clutchout void, the speed is sat for different individual purposes
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
long timeStamp;
uint16_t timeOut = 300; //??
}




//Clutchpressure

namespace clutchpressure
{              
    uint8_t sensorPin = 16;     
    uint8_t sensorValue = 0;
    uint8_t InBar = 0;
    uint8_t engagedThreshold = 2;
    uint8_t disengagedThreshold = 17;
    uint16_t servoRealTimeDegree = 0;
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
const uint16_t brakeActivationPressure = 2;

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
    uint32_t vehicleSpeed = 0;

    }


