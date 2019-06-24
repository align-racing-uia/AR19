
namespace global
{
  const uint8_t sant = 0xF0;
  const uint8_t tull = 0x0F;
}


namespace gearposition          //All variables related to measuring the gear posiiton
{
  const uint8_t sensorPin = 18; // The pin that the sensor in wired to
  uint16_t sensorInput;         // The analog bit value that goes between 1-1024
  uint16_t voltage;             // The sensor bit value remapped to real world voltage
  uint8_t currentGear = 0;      // The gear that corresponds to a specific voltage
  uint8_t newGear = 0;          // Target gear when a gearing process is started
} 


namespace gotoneutral                           //All variables related to measuring the neutral function
{
  long timestamp= 0;                              // Timestamp that restricts the activation and controls the sequence of going to neutral
  const uint16_t timer = 4000;                    // Length of a goToNeutral attempt duartion
  const uint16_t timerLockout = timer + 500;      // Timer to ensure no ther action is started immideatly after going to neutral
}

// Variables for gearing.
namespace gearup
{
  long timestamp = 0;
  const uint8_t pin = 3;
  const uint16_t timer = 1000;                    // Maximim time the proccess of gearing takes - the actuator is on for 200ms less than this value
  const uint16_t timerLockout = timer + 500;      // Lockout period that disallows the system to restart the operation or starn another one.  
}

namespace geardown
{
  long timestamp = 0;
  const uint8_t pin = 10;
  const uint16_t timer = 1000;                    // Maximim time the proccess of gearing takes - the actuator is on for 200ms less than this value
  const uint16_t timerLockout = timer + 500;      // Lockout period that disallows the system to restart the operation or starn another one.  
}



namespace clutchpressure
{              
  const uint8_t sensorPin = 19;     // The pin that the sensor in wired to
  uint16_t sensorValue = 0;         // Bitvalue from the analogRead function
  uint16_t InBar = 0;               // bitvalue remapped to bar in accordance to transducer datasheet
}


namespace clutch
{
  uint8_t servoPin = 9;                     // pin servo - modefied blue led trace
  int16_t engage = 1900;                    // servo position 0 degrees
  int16_t disengage = 1200;                 // servo position 90 degrees
  uint8_t duration = 250;                   //Reset timer for the clutch
  uint8_t emergencyEngage = global::tull;   //Permanently disengages clutch if nautral is not reached.
  long timestamp = 0;                       //timestamp for the duration to be checked againt - is reset by other function in the program
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
        uint8_t gearPositionError = global::tull;
        uint8_t gearChangeFailed = global::tull;
        uint8_t neutralFailed = global::tull;    
    //CAN ID 485 - Action response
        uint8_t actionRecieved = global::tull;
}



// Defines the pins for internal LEDs.
namespace leds
{
    const uint8_t red = 5;
    const uint8_t green = 6;
}
