
void loop() 
{   
CanRecieve();               //Revieves Can messages.

GatherSensorData();         //Updates sensor values.

GearUp();                   //Gears up initial values are set.  // cansignal::GearUp = global::sant

GearDown();                 //Gears down initial values are set // cansignal::GearDown = global::sant

GoToNeutral();              //Goes to neutral if initial values are set // cansignal::neutralSignal = global::sant

ClutchIn();                 //activated when millis() - clutchTimer < clutchDuration

CanSend();                  //sends Can messages

}