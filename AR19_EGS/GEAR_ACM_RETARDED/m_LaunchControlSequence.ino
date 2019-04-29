void launchControlStartUp() {

using namespace gearing;

// Launchcontrol active, car ready to accelerate
if(gearposition::currentgear == 0 && cansignal::launchModeConfirmed == global::tull && millis() - launchTimer > launchTimeStamp)
{
    if(launcModeConfirmed == global::sant)
    {
    servo.write(clutch::disengage);
    launchTimer = millis();
    }
       if(clutchpressure::InBar > 15) //Dersom pressure er 15 bar eller mer så starter kliktronic
      {
        digitalwrite(downPin, HIGH);
      }
        else
        {
            digitalwrite(downPin, LOW);
        }
        
          if(gearposition::currentgear == 1) //Når giret er i pos 1, så avslutter kliktronic og clutch engager
          {
             digitalwrite(downPin, LOW);
             servo.write(clutch::engage);
          }
          else
          {
              digitalwrite(downPin, HIGH);
              servo.write(clutch::disengage);
          }
    else {
        GoToNeutral();
    }      
          
} 

else {
    GoToNeutral();
}


}