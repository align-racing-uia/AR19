void clutchlessDownshift ()
{
using namespace clutchlessdownshift;
 

if ( millis() - gearing::timerGearDown < timeOut)
<<<<<<< HEAD
{
    
    if ( cansignal::breakPressure1 > brakeActivationPressure && cansignal::accPedalPos1 > throttleActivationLimit )
=======
>>>>>>> 3e5cb331d7a8c18751b3e2a12ce1cb6de3a15bbf
    {
    void Blip();
    gearposition::newGear = gearposition::currentGear - 1;
    if( cansignal::blipConfirmed == global::sant && gearposition::currentGear != gearposition::newGear)
      {
        digitalWrite(gearing::downPin, HIGH);
      }
      else
      {
        digitalWrite(gearing::downPin, LOW);
      }
      
    

else
{
  digitalWrite(gearing::downPin, LOW);
}

}
<<<<<<< HEAD
}
=======
>>>>>>> 3e5cb331d7a8c18751b3e2a12ce1cb6de3a15bbf
