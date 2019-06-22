void setup() 
{
  
// define 16 and 17 as input to not interfere with the clutch servo pwm signal
  pinMode(16, INPUT);
  pinMode(17, INPUT);


// set arduino to use a pin as servo control - pin 9
  servo.attach(clutch::servoPin);

//GearCheck  and Clutchpressure senssor pins
  pinMode(gearposition::sensorPin, INPUT);
  pinMode(clutchpressure::sensorPin, INPUT);


// Sets pinmode for gear up and down variables.
  pinMode(gearup::pin, OUTPUT);
  pinMode(geardown::pin, OUTPUT);
  digitalWrite(gearup::pin, LOW);
  digitalWrite(geardown::pin, LOW);


// Sets internal leds.
  pinMode(leds::red, INPUT);
  pinMode(leds::green, INPUT);
  digitalWrite(leds::red,HIGH);
  digitalWrite(leds::green,HIGH);


//Sets up Canbus functionality
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();

}
