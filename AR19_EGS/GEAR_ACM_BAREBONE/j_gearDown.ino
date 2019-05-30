void GearDown()
{
  if (millis()- geardown::timestamp < geardown::timerLockout && millis() > geardown::timerLockout)
  {       
    if (millis()-geardown::timestamp < geardown::timer)
    {
        if(gearposition::currentGear =! gearposition::newGear)
        {
            clutch::timestamp = millis();
        }
    
        if (clutchpressure::InBar > 5 && gearposition::currentGear != gearposition::newGear && millis()- geardown::timestamp < (geardown::timer - 100))
        {
            digitalWrite(geardown::pin, HIGH);    
        }
        if (gearposition::currentGear == gearposition::newGear && millis()- geardown::timestamp > (geardown::timer - 100))
        {
          digitalWrite(geardown::pin, LOW);
          digitalWrite(leds::red,HIGH);
    
        }
        if( gearposition::currentGear != gearposition::newGear && millis() - geardown::timestamp > geardown::timer - 100)
        {
          digitalWrite(geardown::pin, LOW);
          cansignal::gearChangeFailed = global::sant;
          digitalWrite(leds::red,HIGH);
        }
    }

  }

}
