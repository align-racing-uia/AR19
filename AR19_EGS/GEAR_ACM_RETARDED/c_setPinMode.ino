void SetPinMode(){ // Section for initilizing pins
servo.attach(clutch::servoPin); // set arduino to use pin 16 as servo control
//GearCheck pins

    using namespace gearposition;
    {
    pinMode(inputPin, INPUT_PULLUP);
    }


// Sets pinmode for gear up and down variables.


        pinMode(gearup::pin, OUTPUT);
        digitalWrite(gearup::pin, LOW);
        pinMode(geardown::pin, OUTPUT);
        digitalWrite(geardown::pin, LOW);


// Sets internal leds.

    using namespace leds;
    {
        pinMode(red, OUTPUT);
        pinMode(green, OUTPUT);
        pinMode(blue, OUTPUT);
    }

    using namespace clutch;
    {
        servo.attach(servoPin,750,2000); // set arduino to use pin 16 as servo control
    }
}
