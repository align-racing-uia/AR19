
void loop() 
{   
CanRecieve();               //Revieves Can messages.

GatherSensorData()          //Gathers sensor data

Functions();         //Updates sensor values.

CanSend();                  //sends Can messages

}