void CanSetup() // Sets up the CAN-Bus protocol.  
{
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS);
  mcp2515.setNormalMode();

// CAN message 0x15 - ACM OK Signal

  myMessage.can_id = 0x15;  //Henrik: Feil ID på CANbus-melding 
  myMessage.can_dlc = 1; 
  myMessage.data[0] = 21;
  mcp2515.sendMessage(&myMessage); //Signalet blir kun sendt en gang, det er ikke gitt at denne ACM'en er slått på før ACM'en som skal ta imot meldingen

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void CanRecieve() {

using namespace cansignal;
//Ta imot data

 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) 
 {
  
  
  // CAN message 0x410 - Breakpressure.
  if (myMessage.can_id == 0x410)
  {
    breakPressure1 = myMessage.data[0];
  }


  // CAN message 0x230 - Neutral signal.
  if (myMessage.can_id == 0x230) //Henrik: Denne ID'en ligger ikke i registeret
  {
    neutralSignal = myMessage.data[0]; 
  }

  // CAN message 0x240 - Gear paddles.
  if (myMessage.can_id == 0x240)
  {
    gearUpSignal = myMessage.data[0];
    gearDownSignal = myMessage.data[1];  
  }

// CAN message 0x280 - Launch control request clutch.
  if (myMessage.can_id == 0x280)
  {
    launchControlClutch = myMessage.data[0];
  }


// CAN message 0x2C0 - Blip Confirmation from ETC.
  if (myMessage.can_id == 0x2C0)
  {
    blipConfirmed = myMessage.data[0];
  }


// CAN message 0x2F0 - RPM from ECU
  if (myMessage.can_id == 0x2F0) //Henrik: Denne ID'en ligger ikke i registeret
  { 
    rpm1 = myMessage.data[0];
    rpm2 = myMessage.data[1];
  }
engineRPM = rpm1*255 +rpm2;
}

/*
// CAN message 0x210 - Speed from Launch Control - Right front wheel
 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) {
  
    myMessage.can_id = 0x210;  
    myMessage.can_dlc = 4; 
    myMessage.data[0] = speedFrontRight1;
    myMessage.data[1] = speedFrontRight2;
    myMessage.data[3] = speedFrontRight3;
    myMessage.data[3] = speedFrontRight4;
  }
  vehicleSpeed = (SpeedLeftFrontWheel+SpeedRightFrontWheel)/2 ;
speedFrontRightCalculated = speedFrontRight1 + speedFrontRight2 + speedFrontRight3 + speedFrontRight4; ?????????????????????????? hva er dette?
// CAN message 0x220 - Speed from Launch Control - Left front wheel
 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) {
  
    myMessage.can_id = 0x220;  
    myMessage.can_dlc = 4; 
    myMessage.data[0] = speedFrontLeft1;
    myMessage.data[1] = speedFrontLeft2;
    myMessage.data[2] = speedFrontLeft3;
    myMessage.data[3] = speedFrontLeft4;
  }
engineRPM = rpm1*255 +rpm2;
*/

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CanSend() {      // Sende data
using namespace cansignal;
// CAN message 0x20 - div

  myMessage.can_id = 0x20;  
  myMessage.can_dlc = 5; 
  myMessage.data[0] = requestBlip;
  myMessage.data[1] = 0;//engineCut
  myMessage.data[2] = 0;//clutchInSignal 
  myMessage.data[3] = gearPosition; 
  myMessage.data[4] = clutchPressure; 

  mcp2515.sendMessage(&myMessage);

// CAN message 0x470 - Error states to telemetry.
/*
  myMessage.can_id = 0x470;  
  myMessage.can_dlc = 8; 
  myMessage.data[0] = Errorstate;
  myMessage.data[1] = Errorstate;
  myMessage.data[2] = Errorstate; 
  myMessage.data[3] = Errorstate; 
  myMessage.data[4] = Errorstate; 
  myMessage.data[5] = Errorstate; 
  myMessage.data[6] = Errorstate; 
  myMessage.data[7] = Errorstate; 


  mcp2515.sendMessage(&myMessage);
*/


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
