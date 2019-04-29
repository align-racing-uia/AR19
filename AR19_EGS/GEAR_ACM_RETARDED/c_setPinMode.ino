void SetPinMode(){ // Section for initilizing pins
servo.attach(clutch::servoPin); // set arduino to use pin 16 as servo control
//GearCheck pins

    using namespace gearposition;
    {
    pinMode(inputPin, INPUT);
    }


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

    using namespace clutch;
    {
        pinMode(servoPin, OUTPUT); // Usikker om trenges??
        servo.attach(servoPin); // set arduino to use pin 16 as servo control
        servo.write(engage); // output the servoposition to 0
    }
}
