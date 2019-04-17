
// Viktig at denne fila kjem først i kompileringa(etter GEAR_ACM)
    


namespace global{
    const uint8_t sant = 0xF0;
    const uint8_t tull = 0x0F;
    const uint16_t threshold = 500;
}


namespace shutdowncircuit{  // shutdowncircuit -  Declarations
    const uint8_t shutdownPin = 17;  // Make namespace for CheckGear
    uint16_t sensorValue = 0;

} 

// Defines the pins for internal LEDs.
namespace leds{
    const uint8_t red = 5;
    const uint8_t green = 6;
    const uint8_t blue = 9;
}

namespace ambient{
    const uint8_t airTempPin = 16;
    uint8_t airTempSensorValue = 0;
}

namespace turbo{
    const uint8_t airTempPin = 18;
    uint8_t airTempSensorValue = 0;
}

namespace cansignal{
    uint8_t shutdownActive = 0;
    double ambientAirTemp = 0;
    double turboDischargeAirTemp = 0;
    uint8_t helloImHereStartup = 0;
    uint8_t acmOk = 0;
    uint8_t helloImHereStartupPedal = global::tull;
    uint8_t helloImHereStartupHallFL = global::tull;
    uint8_t helloImHereStartupHallFR = global::tull;
    uint8_t helloImHereStartupHALLR = global::tull;
    uint8_t helloImHereStartupLC = global::tull;
    uint8_t helloImHereStartupTelemetri = global::tull;
    uint8_t helloImHereStartupDash = global::tull;
    uint8_t helloImHereStartupETB = global::tull;
    uint8_t helloImHereStartupBreaklight = global::tull;
    uint8_t helloImHereStartupAirtempShutdown = global::tull;
    uint8_t helloImHereStartupStearing = global::tull;
    uint8_t helloImHereStartupStearingWheel = global::tull;
    long acmOkTimer = 0;
    uint8_t pingAllAcm = global::tull;


}
