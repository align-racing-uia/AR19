void CheckDriverIntention()
{
if (cansignal::gearDownSignal == global::sant && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout && millis() - gotoneutral::timestamp > gotoneutral::timerLockout)
    { 
      gearposition::newGear = gearposition::currentGear - 1;
      geardown::timestamp = millis();
      digitalWrite(red,LOW);
    }


if (cansignal::gearUpSignal == global::sant && millis() - geardown::timestamp > geardown::timerLockout && millis() - gearup::timestamp > gearup::timerLockout && millis() - gotoneutral::timestamp > gotoneutral::timerLockout)
  {
    gearposition::newGear = gearposition::currentGear + 1;
    gearup::timestamp = millis();
    digitalWrite(green,LOW);
  } 

if(cansignal::neutralSignal == global::sant && millis() - gotoneutral::timestamp > gotoneutral::timer)
{
    gotoneutral::timestamp = millis();
    digitalWrite(blue,LOW);
}

}
