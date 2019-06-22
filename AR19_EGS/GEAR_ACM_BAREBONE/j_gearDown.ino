void GearDown()
{
  if (millis()- geardown::timestamp < geardown::timer && millis() > geardown::timer)
  {       
    if(gearposition::currentGear != gearposition::newGear/* && cansignal::clutchOverride == global::tull*/)
    {
        clutch::timestamp = millis();
    }
    if (clutchpressure::InBar > 11 /*&& gearposition::currentGear != gearposition::newGear*/ && millis()- geardown::timestamp < (geardown::timer - 200))
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
