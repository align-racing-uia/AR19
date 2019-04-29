
void GearUp(){
using namespace gearing;
  if (cansignal::gearUpSignal == global::sant && millis()-timerGearUp > gearTimerTimeout  && millis()-timerGearDown > gearTimerTimeout && gearposition::currentGear != 6 && millis() - gotoneutral::timerNeutralTimeout > gotoneutral::neutralTimeout + gotoneutral::safeTime && clutchpressure::InBar < clutchpressure::engagedTreshold)
  {
    gearposition::newGear = gearposition::currentGear + 1;
    timerGearUp = millis();
  }
 // geartimer er satt i variables og er omtrent 100ms
  if (millis()- timerGearUp < gearTimer && millis() > gearTimer || millis()-neutraltofirst::timeStamp < neutraltofirst::timeOut && millis() > gearTimer)
  {

    // To gear up from neutral position to first gear the vehicle must stay under 10km/h
      if (gearposition::currentGear == 0 || millis()-neutraltofirst::timeStamp < neutraltofirst::timeOut/*&& cansignal::vehicleSpeed < 10*/)
        {
          NeutralToFirst();        
        }

      else if (gearposition::currentGear >= 1 && gearposition::currentGear <= 5 && cansignal::engineRPM > upshiftRPMLimit )
        {
          digitalWrite(upPin, HIGH);

          if (gearposition::currentGear == gearposition::newGear)
          {
            digitalWrite(upPin, LOW);
            timerGearUp = millis()+gearTimer;
          }
        }

  else if ( millis()- timerGearUp < gearTimerTimeout && gearposition::currentGear >= 1 && gearposition::currentGear <= 5 ) 
    {
      digitalWrite(upPin, LOW);
      digitalWrite(downPin, LOW);
      gearposition::newGear = gearposition::currentGear;
      servo.write(clutch::engage);
    }

  cansignal::gearUpSignal = global::tull;  

  }
}
