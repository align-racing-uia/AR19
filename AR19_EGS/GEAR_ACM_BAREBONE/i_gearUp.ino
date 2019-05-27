void GearUp()
{
  // geartimer er satt i variables og er omtrent 100ms
  if (millis()- gearup::timestamp < gearup::timerLockout && millis() > gearup::timerLockout )
  {       
    if (millis()-gearup::timestamp < gearup::timer)
    {
        if(gearposition::currentGear =! gearposition::newGear)
        {
            clutch::timestamp = millis();
        }
    
        if (clutchpressure::InBar > 5 && gearposition::currentGear != gearposition::newGear && millis()- gearup::timestamp > 200)
        {
            digitalWrite(gearup::pin, HIGH);
          
        }
        else if (gearposition::currentGear == gearposition::newGear millis()- gearup::timestamp > 500)
        {
          digitalWrite(gearup::pin, LOW);
    
        }
    }
    else
    {
      digitalWrite(gearup::pin, LOW);
    }
  }

}
