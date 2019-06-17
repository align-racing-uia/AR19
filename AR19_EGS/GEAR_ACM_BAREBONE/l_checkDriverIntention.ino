void CheckDriverIntention()
{
  if (cansignal::gearDownSignal == global::sant && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout && millis() - gotoneutral::timestamp > gotoneutral::timerLockout)
  { 
    if (/*gearposition::currentGear != 99 && */gearposition::currentGear != 69)
    {
      gearposition::newGear = gearposition::currentGear - 1;
      geardown::timestamp = millis();
      digitalWrite(leds::red,LOW);
      cansignal::gearAttemptInFalsePosition = global::tull;
    }
    else
    {
      cansignal::gearAttemptInFalsePosition = global::sant;
    }
  }

  if (cansignal::gearUpSignal == global::sant && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout && millis() - gotoneutral::timestamp > gotoneutral::timerLockout)
  {
    if (/*gearposition::currentGear != 99 && */gearposition::currentGear != 69)
    {
      gearposition::newGear = gearposition::currentGear + 1;
      gearup::timestamp = millis();
      digitalWrite(leds::green,LOW);
      cansignal::gearAttemptInFalsePosition = global::tull;
    }
    else
    {
      cansignal::gearAttemptInFalsePosition = global::sant;
    }
  } 

  if(cansignal::neutralSignal == global::sant && millis() - gotoneutral::timestamp > gotoneutral::timerLockout && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout)
  {
    if (/*gearposition::currentGear != 99 && */gearposition::currentGear != 69)
    {
      gotoneutral::timestamp = millis();
      digitalWrite(leds::blue,LOW);
      cansignal::gearAttemptInFalsePosition = global::tull;
    }
    else
    {
      cansignal::gearAttemptInFalsePosition = global::sant;
    }
  }

}
