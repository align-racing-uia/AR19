void GearUp()
{
  if (millis()- gearup::timestamp < gearup::timerLockout && millis() > gearup::timerLockout)
  {       
    if (millis()-gearup::timestamp < gearup::timer)
    {
      if(/*gearposition::currentGear =! gearposition::newGear && */cansignal::clutchOverride == global::tull)
      {
          clutch::timestamp = millis();
      }
      if (clutchpressure::InBar > 13 && gearposition::currentGear != gearposition::newGear && millis()- gearup::timestamp < (gearup::timer - 100))
      {
          digitalWrite(gearup::pin, HIGH);    
      }
      else
      {
        digitalWrite(gearup::pin, LOW);  
      }
      if( gearposition::currentGear == gearposition::newGear && millis() - gearup::timestamp > gearup::timer - 100)
      {
        cansignal::gearChangeFailed = global::tull;
        digitalWrite(leds::green,HIGH);
      }
      if( gearposition::currentGear != gearposition::newGear && millis() - gearup::timestamp > gearup::timer - 100)
      {
        cansignal::gearChangeFailed = global::sant;
        digitalWrite(leds::green,HIGH);
      }
    }
  }
}