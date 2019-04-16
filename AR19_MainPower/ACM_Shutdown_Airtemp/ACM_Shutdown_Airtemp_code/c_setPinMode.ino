void SetPinMode(){ // Section for initilizing pins

using namespace shutdowncircuit;
{
    pinMode(shutdownPin, INPUT);
}

// Sets pinmode air temp sensors.

using namespace turbo;
{
    pinMode(airTempPin, INPUT);
}


using namespace turbo;
{
    pinMode(airTempPin, INPUT);
}

// Sets internal leds.

using namespace leds;
{
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(blue, OUTPUT);
}

}
