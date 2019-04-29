void GearDown(){
  // geartimer er satt i variables og er omtrent 100ms
  if (millis()- geardown::timestamp < geardown::timer && millis() > geardown::timer )
  {       

    if(gearposition::currentGear =! gearposition::newGear)
    {
        clutch::timestamp = millis();
    }

    if (clutchpressure::InBar > 5 && gearposition::currentGear != gearposition::newGear)
    {
        digitalWrite(geardown::pin, HIGH);
      
    }
    else if (gearposition::currentGear == gearposition::newGear)
    {
      digitalWrite(geardown::pin, LOW);

    }
  }

  else if(millis()- geardown::timestamp < geardown::timerLockout)
  {
    digitalWrite(geardown::pin, LOW);
  }
  
}
