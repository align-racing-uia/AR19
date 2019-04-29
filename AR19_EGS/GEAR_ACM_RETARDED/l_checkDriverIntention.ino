void CheckDriverIntention()
{
if (cansignal::gearDownSignal == global::sant && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout && millis() - gotoneutral::timestamp > gotoneutral::timerLockout)
    { 
      gearposition::newGear = gearposition::currentGear - 1;
      geardown::timestamp = millis();
      SetGreenLED(true);
      SetGreenLED(false);
      SetBlueLED(false);
    }


if (cansignal::gearDownSignal == global::sant && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout && millis() - gotoneutral::timestamp > gotoneutral::timerLockout)
  {
    gearposition::newGear = gearposition::currentGear + 1;
    gearup::timestamp = millis();
    SetBlueLED(true);
    SetGreenLED(false);
    SetRedLED(false);
  } 

if(cansignal::neutralSignal == global::sant && millis() - gotoneutral::timestamp > gotoneutral::timer)
{
    gotoneutral::timestamp = millis();
    SetRedLED(true);
    SetGreenLED(false);
    SetBlueLED(false);
}

}
