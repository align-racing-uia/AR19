void CanSetup() // Sets up the CAN-Bus protocol.  
{
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS);
  mcp2515.setNormalMode();

// CAN message 0x15 - ACM OK Signal

  myMessage.can_id = 0x15;  
  myMessage.can_dlc = 1; 
  myMessage.data[0] = 21;


  mcp2515.sendMessage(&myMessage);

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void CanRecieve() {

using namespace cansignal;
//Ta imot data

// CAN message 0x410 - Breakpressure.

 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) {
  
    myMessage.can_id = 0x410;  // ID'en til meldingen som er mottat
    myMessage.can_dlc = 3; // Antal byte med data i meldingen 1 til 8
    myMessage.data[0] = breakPressure1; // Det første bytet med data
  }


// CAN message 0x230 - Neutral signal.

 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) {
  
    myMessage.can_id = 0x230;  
    myMessage.can_dlc = 1; 
    myMessage.data[0] = neutralSignal; 
  }

// CAN message 0x240 - Gear paddles.

 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) {
  
    myMessage.can_id = 0x240;  
    myMessage.can_dlc = 2; 
    myMessage.data[0] = gearUpSignal;
    myMessage.data[0] = gearDownSignal;  
  }

// CAN message 0x280 - Launch control request clutch.

 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) {
  
    myMessage.can_id = 0x280;  
    myMessage.can_dlc = 1; 
    myMessage.data[0] = launchControlClutch;
  }



// CAN message 0x2C0 - Blip Confirmation from ETC.
 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) {
  
    myMessage.can_id = 0x2C0;  
    myMessage.can_dlc = 1; 
    myMessage.data[0] = blipConfirmed;
  }


// CAN message 0x2F0 - RPM from ECU
 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) {
  
    myMessage.can_id = 0x2F0;  
    myMessage.can_dlc = 2; 
    myMessage.data[0] = rpm1;
    myMessage.data[1] = rpm2;
  }
engineRPM = rpm1*255 +rpm2;

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
speedFrontRightCalculated = speedFrontRight1 + speedFrontRight2 + speedFrontRight3 + speedFrontRight4;
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
