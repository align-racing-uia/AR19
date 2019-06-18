
// Viktig at denne fila kjem først i kompileringa(etter GEAR_ACM)


namespace global{
    const uint8_t sant = 0xF0;
    const uint8_t tull = 0x0F;
}


namespace gearposition{  // CheckGear -  Declarations
    const uint8_t sensorPin = 16;  // Make namespace for CheckGear
    uint16_t sensorInput;
    uint16_t voltage;
    uint8_t currentGear = 0;
    uint8_t newGear = 0;

} 


namespace gotoneutral
{
long timestamp= 0;
const uint16_t timer = 4000; // Length of a goToNeutral attempt duartion
const uint16_t timerLockout = 5000; 


}

// Variables for gearing.


namespace gearup{
    long timestamp = 0;
    const uint8_t pin = 3;
    const uint16_t timer = 400;
    const uint16_t timerLockout = 500;

}

namespace geardown{
    long timestamp = 0;
    const uint8_t pin = 17;
    const uint16_t timer = 400;
    const uint16_t timerLockout = 500;

}



//Clutchpressure

namespace clutchpressure
{              
    const uint8_t sensorPin = 19;     
    uint16_t sensorValue = 0;
    uint16_t InBar = 0;

}


//Active clutch



namespace clutch
{
    uint8_t servoPin = 10; // pin servo - egentlig pin 10
    int16_t engage = 1900; // servo position 0 degrees
    int16_t disengage = 1200; // servo position 90 degrees
    uint8_t duration = 160; //Reset timer for the clutch
    uint8_t emergencyEngage = global::tull;
    long timestamp = 0;

}

namespace cansignal
{
// Variables derived from the CAN-Bus
//Timers
    unsigned long telemetryTimer = 0;
    unsigned long primaryTimer = 0;
//Recieving
    //Steering Wheel ACM
        uint8_t gearUpSignal = global::tull;
        uint8_t gearDownSignal = global::tull;
    //Dash ACM
        uint8_t neutralSignal = global::tull;
        uint8_t clutchOverride = global::tull;
//Sending
    //CAN ID 20
        //uint8_t requestBlip = global::tull;
    //CAN ID 21
        // Frame 0 : Gear Position
        // Frame 1 : Clutch Pressure
    //CAN ID 480 - Telemetry
        uint8_t clutchPressureError = global::tull;
        uint8_t gearAttemptInFalsePosition = global::tull;
        uint8_t gearPositionError = global::tull;
        uint8_t gearChangeFailed = global::tull;
        uint8_t neutralFailed = global::tull;    
}



// Defines the pins for internal LEDs.
namespace leds
{
    const uint8_t red = 5;
    const uint8_t green = 6;
    const uint8_t blue = 9;
}
