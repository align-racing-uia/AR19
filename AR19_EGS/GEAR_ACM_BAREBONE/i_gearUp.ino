void GearUp()
{
  if (millis()- gearup::timestamp < gearup::timerLockout && millis() > gearup::timerLockout)
  {       
    if (millis()-gearup::timestamp < gearup::timer)
    {
        if(gearposition::currentGear =! gearposition::newGear)
        {
            clutch::timestamp = millis();
        }
    
        if (clutchpressure::InBar > 5 && gearposition::currentGear != gearposition::newGear && millis()- gearup::timestamp < (gearup::timer - 100))
        {
            digitalWrite(gearup::pin, HIGH);    
        }
        if (gearposition::currentGear == gearposition::newGear && millis()- gearup::timestamp > (gearup::timer - 100))
        {
          digitalWrite(gearup::pin, LOW);
          digitalWrite(leds::green,HIGH);
    
        }
        if( gearposition::currentGear != gearposition::newGear && millis() - gearup::timestamp > gearup::timer - 100)
        {
          digitalWrite(gearup::pin, LOW);
          cansignal::gearChangeFailed = global::sant;
          digitalWrite(leds::green,HIGH);
        }
    }

  }

}