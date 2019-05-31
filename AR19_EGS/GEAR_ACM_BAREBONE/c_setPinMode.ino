void SetPinMode(){ // Section for initilizing pins
servo.attach(clutch::servoPin); // set arduino to use pin 16 as servo control

//GearCheck pins
    pinMode(gearposition::sensorPin, INPUT);
    
//Clutchpressure pins
    pinMode(clutchpressure::sensorPin, INPUT);

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
        digitalWrite(red,HIGH);
        digitalWrite(green,HIGH);
        digitalWrite(blue,HIGH);
    }

    using namespace clutch;
    {
        servo.attach(servoPin);
    }

    
}
