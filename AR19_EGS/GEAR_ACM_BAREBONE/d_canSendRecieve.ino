void CanSetup() // Sets up the CAN-Bus protocol.  
{
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();

// CAN message 0x15 - ACM OK Signal

  myMessage.can_id = 0x17;  //Henrik:Feil ID på CANbus-melding , fikset
  
  myMessage.can_dlc = 1; 
  myMessage.data[0] = global::sant;
  mcp2515.sendMessage(&myMessage); // Signalet blir kun sendt en gang, det er ikke gitt at denne ACM'en er 

}


void CanRecieve() {
  
using namespace cansignal;


//Ta imot data

 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) 
 {

  // CAN message 0x42 - Neutral signal and clutch override.
  if (myMessage.can_id == 0x42)
  {
    neutralSignal = myMessage.data[0];
    clutchOverride =  myMessage.data[1];
  }

  // CAN message 0x240 - Gear paddles.
  if (myMessage.can_id == 0x240)
  {
    gearDownSignal  = myMessage.data[0];
    gearUpSignal    = myMessage.data[1];  
  }
  else
  {
    gearDownSignal = global::tull;
    gearUpSignal = global::tull;    
  }



  //Responds to pings
  if (myMessage.can_id == 0x19 && bitRead(myMessage.data[0],5) == 1)
  { 
  myMessage.can_id = 0x17;  //Henrik: Feil ID, fikset
  myMessage.can_dlc = 1; 
  myMessage.data[0] = global::sant;
  mcp2515.sendMessage(&myMessage);
  }

 }


}

void CanSend() {      // Sende data
using namespace cansignal;

// CAN message 0x21 - primary messages

  if (primaryTimer - millis() > 20)

  {
    myMessage.can_id = 0x21;  
    myMessage.can_dlc = 2; 
    myMessage.data[0] = gearposition::currentGear; 
    myMessage.data[1] = clutchpressure::InBar; 
    
    mcp2515.sendMessage(&myMessage);
    primaryTimer = millis();
  }

// CAN message 0x480 - Error states to telemetry.

  if((clutchPressureError == global::sant || gearAttemptInFalsePosition == global::sant || gearPositionError == global::sant || gearChangeFailed == global::sant || neutralFailed == global::sant) && (telemetryTimer - millis() > 20))
  {
    myMessage.can_id  = 0x480;  
    myMessage.can_dlc = 5; 
    myMessage.data[0] = clutchPressureError; //Errorstate - Clutch Pressure out of range
    myMessage.data[1] = gearAttemptInFalsePosition;//Errorstate - Signal to change gear was sent when gear position was undefined.
    myMessage.data[2] = gearPositionError; //Sends telemetry signal if gear position sensor is invalid
    myMessage.data[3] = gearChangeFailed; //The desired gear was not reached within the given period
    myMessage.data[4] = neutralFailed; //sens error message that neutral attempt failed //Henrik: ID'en ligger ikke i registeret

    mcp2515.sendMessage(&myMessage);
    telemetryTimer = millis();
  }

    // CAN message 0x1 - Activate shutdown circuit
    
  if((clutchPressureError == global::sant || gearPositionError == global::sant) && (shutdownTimer - millis() > 20))
  {
    myMessage.can_id  = 0x1;  
    myMessage.can_dlc = 1; 
    myMessage.data[0] = global::sant; // Activates the shutdown circuit

    mcp2515.sendMessage(&myMessage);
    shutdownTimer = millis();
  }
}
