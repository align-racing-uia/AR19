void GearDown(){
  using namespace gearing;
  if (cansignal::gearDownSignal == global::sant && millis()-timerGearDown > gearTimerTimeout && millis()-timerGearUp > gearTimerTimeout && gearposition::currentGear >= 2 && cansignal::engineRPM < downshiftRPMLimit && millis() - gearing::timerNeutralTimeout > gearing::neutralTimeout/*&& clutchpressure::InBar < clutchpressure::engagedTreshold*/)
    {
      timerGearDown = millis();
       gearposition::newGear = gearposition::currentGear - 1;
    }
  // geartimer er satt i variables og er omtrent 100ms
  if (millis()- timerGearDown < gearTimer && millis() > gearTimer )
  {       

    if(gearposition::currentGear =! gearposition::newGear)
    {
      servo.write(clutch::disengage); // Sets the servo to 90 degrees position
    }
    if (clutchpressure::InBar >= clutchpressure::disengagedThreshold && gearposition::currentGear != gearposition::newGear)
    {
        cansignal::requestBlip = global::sant;

      if(cansignal::blipConfirmed == global::sant && timerBlipGearingDelay == 0)
      {
        timerBlipGearingDelay = millis();
      }
      if ( millis() - timerBlipGearingDelay > blipGearingDelay)
      {
        digitalWrite(downPin, HIGH);
        cansignal::requestBlip = global::tull;
      }
    }
    if (gearposition::currentGear == gearposition::newGear && clutchpressure::InBar > 1)
    {
      digitalWrite(downPin, LOW);
      void variableClutchEngage (downshiftClutchEngageSpeed);
      timerBlipGearingDelay = 0;
    }
  }

  else if(millis()- timerGearDown < gearTimerTimeout )
  {
    digitalWrite(downPin, LOW);
    digitalWrite(upPin, LOW);
    servo.write(clutch::engage);
    cansignal::blipConfirmed == global::tull;
    timerBlipGearingDelay = 0;
  }
  
  
  cansignal::gearDownSignal = global::tull;  
  
}


///////////////////////// Is to be included if using clutcless downshift
/*
    if( millis() - timerGearDown < cLsActivationTimeLimit && cansignal::breakPressure1 > clutchlessdownshift::brakeActivationPressure || millis() - timerGearDown < clutchlessdownshift::timeOut )
    {
    clutchlessDownshift();
    }
    */
