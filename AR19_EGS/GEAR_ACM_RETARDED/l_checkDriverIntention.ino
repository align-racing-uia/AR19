void CheckDriverIntention()
{
if (cansignal::gearDownSignal == global::sant && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout && millis() - gotoneutral::timestamp > gotoneutral::timerLockout)
    { 
      gearposition::newGear = gearposition::currentGear - 1;
      geardown::timestamp = millis();
    }


if (cansignal::gearDownSignal == global::sant && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout && millis() - gotoneutral::timestamp > gotoneutral::timerLockout)
  {
    gearposition::newGear = gearposition::currentGear + 1;
    gearup::timestamp = millis();
  } 

if(cansignal::neutralSignal == global::sant && millis() - gotoneutral::timestamp > gotoneutral::timer)
{
    gotoneutral::timestamp = millis();
}

}