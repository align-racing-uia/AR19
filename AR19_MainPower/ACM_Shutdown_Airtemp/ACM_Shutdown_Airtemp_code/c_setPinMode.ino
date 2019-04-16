void SetPinMode(){ // Section for initilizing pins


// Sets pinmode for gear up and down variables.

    using namespace gearing;
        {
        pinMode(upPin, OUTPUT);
        pinMode(downPin, OUTPUT);
        digitalWrite(upPin, LOW);
        digitalWrite(downPin , LOW);
        }

// Sets internal leds.

    using namespace leds;
    {
        pinMode(red, OUTPUT);
        pinMode(green, OUTPUT);
        pinMode(blue, OUTPUT);
    }

}
