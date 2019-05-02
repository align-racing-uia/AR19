void CanSetup() // Sets up the CAN-Bus protocol.  
{
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS);
  mcp2515.setNormalMode();

// CAN message 0x15 - ACM OK Signal

  myMessage.can_id = 0x15;  
  myMessage.can_dlc = 1; 
  myMessage.data[0] = global::sant;
  mcp2515.sendMessage(&myMessage);

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void CanRecieve() {
  
using namespace cansignal;
gearUpSignal = global::tull;  
gearDownSignal = global::tull;  
neutralSignal = global::tull;
clutchOverride = global::tull;


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
    gearUpSignal = myMessage.data[0];
    gearDownSignal = myMessage.data[1];  
  }

  // CAN message 0x2F0 - RPM from ECU
  if (myMessage.can_id == 0x2F0)
  { 
    rpm1 = myMessage.data[0];
    rpm2 = myMessage.data[1];
    engineRPM = rpm1*255 +rpm2;
  }




  //Responds to pings
  if (myMessage.can_id == 0x19 && bitRead(myMessage.data[0],5) == 1)
  { 
  myMessage.can_id = 0x15;  
  myMessage.can_dlc = 1; 
  myMessage.data[0] = global::sant;
  mcp2515.sendMessage(&myMessage);
  }

 }


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CanSend() {      // Sende data
using namespace cansignal;
// CAN message 0x20 - div

  myMessage.can_id = 0x20;  
  myMessage.can_dlc = 5; 
  myMessage.data[0] = 0;//requestBlip;
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
