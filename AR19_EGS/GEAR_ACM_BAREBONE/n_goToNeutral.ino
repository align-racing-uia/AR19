void GoToNeutral()
{
  if (millis()-gotoneutral::timestamp < gotoneutral::timerLockout && millis() > gotoneutral::timerLockout)
  {
    clutch::timestamp = millis();
    if (clutchpressure::InBar > 5 && millis() - gotoneutral::timestamp < gotoneutral::timer && millis() - gotoneutral::timestamp < (gotoneutral::timer - 100) && gearposition::currentGear != 0)
    {             
        if (gearposition::currentGear == 2)
        {
            digitalWrite(geardown::pin, HIGH);
            digitalWrite(gearup::pin, LOW);
        }
        else if (gearposition::currentGear == 1)
        {
            digitalWrite(gearup::pin, HIGH);
            digitalWrite(geardown::pin, LOW);
        }
        else if (gearposition::currentGear > 2)
        {
            digitalWrite(geardown::pin, HIGH);
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









