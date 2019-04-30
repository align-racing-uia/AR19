void SetPinMode(){ // Section for initilizing pins

using namespace shutdowncircuit;
{
    pinMode(sensePin, INPUT_PULLUP);
    pinMode(activatePin, OUTPUT);
    digitalWrite(activatePin, HIGH)
    
}

// Sets pinmode air temp sensors.

using namespace turbo;
{
    pinMode(airTempPin, OUTPUT);
}


using namespace turbo;
{   
    pinMode(airTempPin, OUTPUT);
}

// Sets internal leds.

using namespace leds;
{
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(blue, OUTPUT);
}

}
