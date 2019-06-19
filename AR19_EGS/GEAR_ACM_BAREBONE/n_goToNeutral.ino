void GoToNeutral()
{
  if (millis()-gotoneutral::timestamp < gotoneutral::timer && millis() > gotoneutral::timer)
  {
    clutch::timestamp = millis();
    if (clutchpressure::InBar > 11 && millis() - gotoneutral::timestamp < gotoneutral::timer && millis() - gotoneutral::timestamp < (gotoneutral::timer - 1000)/* && gearposition::currentGear != 0*/)
    {             
        if (gearposition::currentGear == 2)
        {
            digitalWrite(geardown::pin, HIGH);
            digitalWrite(gearup::pin, LOW);   // Just to make sure the actuator doesnt acuate both ways
            delay(20);//Henrik: Ikke bruk delay(), dette kan føre til at can-meldinger ikke kommer frem
            digitalWrite(geardown::pin, LOW);
            delay(250);//Henrik: Ikke bruk delay(), dette kan føre til at can-meldinger ikke kommer frem
        }
        else if (gearposition::currentGear == 1)
        {
            digitalWrite(gearup::pin, HIGH);
            digitalWrite(geardown::pin, LOW); // Just to make sure the actuator doesnt acuate both ways
            delay(20);//Henrik: Ikke bruk delay(), dette kan føre til at can-meldinger ikke kommer frem
            digitalWrite(gearup::pin, LOW);
            delay(250);//Henrik: Ikke bruk delay(), dette kan føre til at can-meldinger ikke kommer frem
        }
        else if (gearposition::currentGear >= 3 && gearposition::currentGear <= 6)
        {
            digitalWrite(geardown::pin, HIGH);
            digitalWrite(gearup::pin, LOW); // Just to make sure the actuator doesnt acuate both ways
            delay(100);//Henrik: Ikke bruk delay(), dette kan føre til at can-meldinger ikke kommer frem
            digitalWrite(geardown::pin, LOW);
            delay(250);//Henrik: Ikke bruk delay(), dette kan føre til at can-meldinger ikke kommer frem
        }
        else
        {
          digitalWrite(geardown::pin, LOW);
          digitalWrite(gearup::pin, LOW);
        }
        
    }
    if (gearposition::currentGear == 0/* && millis() - gotoneutral::timestamp > (gotoneutral::timer - 300)*/)
    {
      digitalWrite(geardown::pin, LOW);
      digitalWrite(gearup::pin, LOW);
      digitalWrite(leds::green,HIGH);
      digitalWrite(leds::red,HIGH);
      clutch::emergencyEngage = global::tull;
	    cansignal::neutralFailed = global::tull;
    }
    if (gearposition::currentGear != 0 && millis() - gotoneutral::timestamp > (gotoneutral::timer - 500))
    {
      digitalWrite(geardown::pin, LOW);
      digitalWrite(gearup::pin, LOW);
      digitalWrite(leds::red,LOW);
      digitalWrite(leds::green,LOW);
      cansignal::neutralFailed = global::sant;
      clutch::emergencyEngage = global::sant;
    }
  }
}
