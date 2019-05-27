
void loop() 
{   
    CanRecieve();               //Revieves Can messages.

    GatherSensorData();         //Updates sensor values.

    CheckDriverIntention();

    GearUp();                   //Gears up initial values are set.  // cansignal::GearUp = global::sant

    GearDown();                 //Gears down initial values are set // cansignal::GearDown = global::sant

    GoToNeutral();              //Goes to neutral if initial values are set // cansignal::neutralSignal = global::sant

    ClutchIn();                 //activated when millis() - clutchTimer < clutchDuration

    if (millis()-cansignal::timer > 100)
    {
        CanSend();                  //sends Can messages
        cansignal::timer = millis();
    }
}
