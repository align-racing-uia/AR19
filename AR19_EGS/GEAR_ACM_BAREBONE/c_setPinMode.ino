void SetPinMode(){ // Section for initilizing pins

// define 16 and 17 as input
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  
servo.attach(clutch::servoPin); // set arduino to use a pin as servo control

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
        pinMode(red, INPUT);
        pinMode(green, INPUT);
        pinMode(blue, INPUT);
        
        //digitalWrite(red,HIGH);
        //digitalWrite(green,HIGH);
        //digitalWrite(blue,HIGH);
    }

    using namespace clutch;
    {
        servo.attach(servoPin);
    }

    
}
