void GearDown()
{
  // geartimer er satt i variables og er omtrent 100ms
  if (millis()- geardown::timestamp < geardown::timerLockout && millis() > geardown::timerLockout)
  {       
    if (millis()-geardown::timestamp < geardown::timer)
    {
        if(gearposition::currentGear =! gearposition::newGear)
        {
            clutch::timestamp = millis();
        }
    
        if (clutchpressure::InBar > 5 && gearposition::currentGear != gearposition::newGear && millis()- geardown::timestamp > 200)
        {
            digitalWrite(geardown::pin, HIGH);    
        }
        else if (gearposition::currentGear == gearposition::newGear || millis()- geardown::timestamp > 500)
        {
          digitalWrite(geardown::pin, LOW);
    
        }
    }
    else
    {
      digitalWrite(geardown::pin, LOW);
      cansignal::gearChangeFailed = global::sant;
    }
  }

}
