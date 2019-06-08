void GearDown()
{
  if (millis()- geardown::timestamp < geardown::timerLockout && millis() > geardown::timerLockout)
  {       
    if (millis()-geardown::timestamp < geardown::timer)
    {
      if(/*gearposition::currentGear =! gearposition::newGear && */cansignal::clutchOverride == global::tull)
      {
          clutch::timestamp = millis();
      }
      if (clutchpressure::InBar > 13 && gearposition::currentGear != gearposition::newGear && millis()- geardown::timestamp < (geardown::timer - 100))
      {
          digitalWrite(geardown::pin, HIGH);    
      }
      else
      {
        digitalWrite(geardown::pin, LOW);   
      }
      if( gearposition::currentGear == gearposition::newGear && millis() - geardown::timestamp > geardown::timer - 100)
      {
        cansignal::gearChangeFailed = global::tull;
        digitalWrite(leds::red,HIGH);
      }
      if( gearposition::currentGear != gearposition::newGear && millis() - geardown::timestamp > geardown::timer - 100)
      {
        cansignal::gearChangeFailed = global::sant;
        digitalWrite(leds::red,HIGH);
      }
    }
  }
}