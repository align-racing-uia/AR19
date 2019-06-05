void GoToNeutral()
{
  if (millis()-gotoneutral::timestamp < gotoneutral::timerLockout && millis() > gotoneutral::timerLockout)
  {
    clutch::timestamp = millis();
    if (clutchpressure::InBar > 13 && millis() - gotoneutral::timestamp < gotoneutral::timer && millis() - gotoneutral::timestamp < (gotoneutral::timer - 100) && gearposition::currentGear != 0)
    {             
        if (gearposition::currentGear == 2)
        {
            digitalWrite(geardown::pin, HIGH);
            digitalWrite(gearup::pin, LOW);   // Just to make sure the actuator doesnt acuate both ways
            delay(10)
            digitalWrite(geardown::pin, LOW);
            delay(100)
        }
        else if (gearposition::currentGear == 1)
        {
            digitalWrite(gearup::pin, HIGH);
            digitalWrite(geardown::pin, LOW); // Just to make sure the actuator doesnt acuate both ways
            delay(10)
            digitalWrite(gearup::pin, LOW);
            delay(100)
        }
        else if (gearposition::currentGear >= 3 && gearposition::currentGear <= 6)
        {
            digitalWrite(geardown::pin, HIGH);
            digitalWrite(gearup::pin, LOW); // Just to make sure the actuator doesnt acuate both ways
            delay(10)
            digitalWrite(geardown::pin, LOW);
            delay(100)
        }
        else
        {
          digitalWrite(geardown::pin, LOW);
          digitalWrite(gearup::pin, LOW);
        }
        
    }
    if (gearposition::currentGear == 0 && millis() - gotoneutral::timestamp > (gotoneutral::timer - 100))
    {
      digitalWrite(geardown::pin, LOW);
      digitalWrite(gearup::pin, LOW);
      digitalWrite(leds::blue,HIGH);
      digitalWrite(leds::green,HIGH);
      digitalWrite(leds::red,HIGH);
      clutch::emergencyEngage = global::tull;
    }
    if (gearposition::currentGear != 0 && millis() - gotoneutral::timestamp > (gotoneutral::timer - 100))
    {
      digitalWrite(geardown::pin, LOW);
      digitalWrite(gearup::pin, LOW);
      digitalWrite(leds::blue,LOW);
      digitalWrite(leds::red,LOW);
      digitalWrite(leds::green,LOW);
      cansignal::neutralFailed = global::sant;
      clutch::emergencyEngage = global::sant;
    }
  }
}









