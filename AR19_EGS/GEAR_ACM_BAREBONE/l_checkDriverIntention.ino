void CheckDriverIntention()
{
  if (cansignal::gearDownSignal == global::sant && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout && millis() - gotoneutral::timestamp > gotoneutral::timerLockout)
  { 
    if (gearposition::currentGear != 99)
    {
      gearposition::newGear = gearposition::currentGear - 1;
      geardown::timestamp = millis();
      digitalWrite(red,LOW);
      cansignal::gearAttemptInFalsePosition = global::tull;
    }
    else
    {
      cansignal::gearAttemptInFalsePosition = global::sant;
    }
    

  }

  if (cansignal::gearUpSignal == global::sant && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout && millis() - gotoneutral::timestamp > gotoneutral::timerLockout)
  {
    if (gearposition::currentGear != 99)
    {
      gearposition::newGear = gearposition::currentGear + 1;
      gearup::timestamp = millis();
      digitalWrite(green,LOW);
      cansignal::gearAttemptInFalsePosition = global::tull;
    }
    else
    {
      cansignal::gearAttemptInFalsePosition = global::sant;
    }
  } 

  if(cansignal::neutralSignal == global::sant && millis() - gotoneutral::timestamp > gotoneutral::timer)
  {
    if (gearposition::currentGear != 99)
    {
      gotoneutral::timestamp = millis();
      digitalWrite(blue,LOW);
          cansignal::gearAttemptInFalsePosition = global::tull;
    }
    else
    {
      cansignal::gearAttemptInFalsePosition = global::sant;
    }
  }

}