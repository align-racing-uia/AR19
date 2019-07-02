void CheckDriverIntention()
{
  if (cansignal::gearDownSignal == global::sant && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout && millis() - gotoneutral::timestamp > gotoneutral::timerLockout)
  { 
      gearposition::newGear = gearposition::currentGear - 1;
      geardown::timestamp = millis();
      digitalWrite(leds::red,LOW);
  }

  if (cansignal::gearUpSignal == global::sant && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout && millis() - gotoneutral::timestamp > gotoneutral::timerLockout)
  {
      
      gearposition::newGear = gearposition::currentGear + 1;
      gearup::timestamp = millis();
      digitalWrite(leds::green,LOW);
/*
      if(gearposition::currentGear <= 1)
      {
        SecondGearFix();
      }

      else
      {
        gearposition::newGear = gearposition::currentGear + 1;
        gearup::timestamp = millis();
        digitalWrite(leds::green,LOW);
      }

*/

  } 

  if(cansignal::neutralSignal == global::sant && millis() - gotoneutral::timestamp > gotoneutral::timerLockout && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout)
  {
      gotoneutral::timestamp = millis();
      digitalWrite(leds::red,LOW);
      digitalWrite(leds::green,LOW);
  }

}
