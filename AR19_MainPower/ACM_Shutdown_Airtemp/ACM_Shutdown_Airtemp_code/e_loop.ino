
void loop() 
{   
PingAcm();                  //Pings all ACM if one or more is missing.

CanRecieve();               //Revieves Can messages.

CheckShutdownStatus();      //Checks if the shutdowncircuit is active

CheckAirTempAmbient();      //Measures Ambient Temperature 

CheckAirTempTurbo();        //Measures the turbo pressure       

ShutdownActivation();       //Activated the shutdown circuit if conditions are met. 

CanSend();                  //sends Can messages

}