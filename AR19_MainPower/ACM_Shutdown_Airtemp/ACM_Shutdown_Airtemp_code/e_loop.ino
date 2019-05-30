
void loop() 
{   
PingAcm();                  //Pings all ACM if one or more is missing.

CanRecieve();               //Revieves Can messages.

CheckShutdownStatus();      //Checks if the shutdowncircuit is active

CheckAirTempAmbient();      //Measures Ambient Temperature 

CheckAirTempTurbo();        //Measures the turbo pressure       

if (millis()-cansignal::sendTimer > 100)
{
    CanSend();                  //sends Can messages
    cansignal::sendTimer = millis();
}


if (millis() - shutdowncircuit::lockoutTimer < 300)
{
  digitalWrite(shutdowncircuit::activatePin, LOW);
  digitalWrite(leds::blue,LOW);
}
else
{
  digitalWrite(shutdowncircuit::activatePin, HIGH);
  digitalWrite(leds::blue,HIGH);
}
  
}
