void LaunchControl()
{
  if (millis() - launch::timestamp < launch::timer && millis() > launch::timer)
  {
    if (cansignal::launchSignal == 0xF0)              //stand ready signal from LC
    {
      servo.writeMicroseconds(clutch::disengage);             //Disconnects the engine and wheels

      launch::active = global::sant;

      launch::clutchposition = clutch::disengage;
      
      if (gearposition::currentGear != 1)
      {
        cansignal::gearDownSignal == global::sant;
      }
    }
    
    if(cansignal::launchSignal == 0x0F && launch::active == global::sant && gearposition::currentGear == 1)               //Launch signal
    {
      servo.writeMicroseconds(launch::clutchposition);
      if( launch::clutchposition < clutch::engage)
      {
        launch::clutchposition = launch::clutchposition +1;
      }
      else
      {
        launch::timestamp = millis()+launch::timer;
      }
    }

    if(cansignal::launchSignal == 0xAA)               //Launch abortion signal
    {
      gotoneutral::timestamp = millis();
      launch::active = global::tull;
    }
    
  }

  else
  {
    launch::active = global::tull;
  }
}
