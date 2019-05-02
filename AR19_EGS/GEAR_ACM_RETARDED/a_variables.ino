
// Viktig at denne fila kjem først i kompileringa(etter GEAR_ACM)


namespace global{
    const uint8_t sant = 0xF0;
    const uint8_t tull = 0x0F;
}


namespace gearposition{  // CheckGear -  Declarations
    const uint8_t inputPin = 116;  // Make namespace for CheckGear
    uint8_t sensorInput;
    uint8_t voltage;
    uint8_t currentGear = 0;
    uint8_t newGear = 0;

} 


namespace gotoneutral
{
long timestamp= 0;
const uint16_t timer = 2000;// Length of a goToNeutral attempt duartion
const uint16_t timerLockout = 2500; 


}

// Variables for gearing.


namespace gearup{
    long timestamp = 0;
    const uint8_t pin = 3;
    const uint16_t timer = 200;
    const uint16_t timerLockout = 1000;

}

namespace geardown{
    long timestamp = 0;
    const uint8_t pin = 17;
    const uint16_t timer = 200;
    const uint16_t timerLockout = 1000;

}



//Clutchpressure

namespace clutchpressure
{              
    uint8_t sensorPin = 19;     
    uint8_t sensorValue = 0;
    uint8_t InBar = 0;

}


//Active clutch



namespace clutch{
    uint8_t servoPin = 3; // pin servo - egentlig pin 3
    int16_t engage = 0; // servo position 0 degrees
    int16_t disengage = 90; // servo position 90 degrees
    long timestamp = 0;
    uint8_t duration = 30; //how long the clutch takes to engage.

}


// Variables derived from the CAN-Bus
namespace cansignal{
    uint8_t breakPressure1 = 0;
    uint8_t gearUpSignal = global::tull;
    uint8_t gearDownSignal = global::tull;
    uint8_t gearPosition = 0;
    uint8_t rpm1 = 0;
    uint8_t rpm2 = 0;
    uint16_t engineRPM = 0;
    uint8_t clutchPressure = 0;
    uint8_t neutralSignal = global::tull;
    uint8_t clutchOverride = global::tull;

    }



// Defines the pins for internal LEDs.
namespace leds{
    const uint8_t red = 5;
    const uint8_t green = 6;
    const uint8_t blue = 9;
}
