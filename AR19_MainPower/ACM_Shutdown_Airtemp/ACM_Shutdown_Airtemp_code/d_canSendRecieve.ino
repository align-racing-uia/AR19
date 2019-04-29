void CanSetup() // Sets up the CAN-Bus protocol.  
{
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS);
  mcp2515.setNormalMode();


  //mcp2515.sendMessage(&myMessage);

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void CanRecieve() {

using namespace cansignal;
//Ta imot data

 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) {
  
    if (myMessage.can_id == 0x1 || millis() - shutdowncircuit::lockoutTimer > 300)
    {
      digitalWrite(shutdowncircuit::activatepin, LOW)
    }
    else
    {
      digitalWrite(shutdowncircuit::activatepin, HIGH)
    }


    if (myMessage.can_id == 0x12 && myMessage.can_dlc == 1)
    {
      helloImHereStartupPedal = myMessage.data[0]; 
      if (helloImHereStartupPedal == global::sant)
      {
        bitWrite(acmOk,1,1);
      }
      else
      {
        bitWrite(acmOk,1,0);
      }
    }

    else if (myMessage.can_id == 0x13)
    {
      helloImHereStartupDash = myMessage.data[0]; 
      if (helloImHereStartupDash == global::sant)
      {
        bitWrite(acmOk,2,1);
      }
      else
      {
        bitWrite(acmOk,2,0);
      }
    }

    else if (myMessage.can_id == 0x14) 
    {
      helloImHereStartupStearingWheel = myMessage.data[0]; 
      if (helloImHereStartupStearingWheel == global::sant)
      {
        bitWrite(acmOk,3,1);
      }
      else
      {
        bitWrite(acmOk,3,0);
      }
    }


    else if (myMessage.can_id == 0x15) 
    {
      helloImHereStartupLC = myMessage.data[0]; 
      if (helloImHereStartupLC == global::sant)
      {
        bitWrite(acmOk,4,1);
      }
      else
      {
        bitWrite(acmOk,4,0);
      }
    }


    else if (myMessage.can_id == 0x16)
    {
      helloImHereStartupETB = myMessage.data[0]; 
      if (helloImHereStartupETB == global::sant)
      {
        bitWrite(acmOk,5,1);
      }
      else
      {
        bitWrite(acmOk,5,0);
      }
    }


    else if (myMessage.can_id == 0x17)
    {
      helloImHereStartupEGS = myMessage.data[0]; 
      if (helloImHereStartupEGS == global::sant)
      {
        bitWrite(acmOk,6,1);
      }
      else
      {
        bitWrite(acmOk,6,0);
      }
    }


    else if (myMessage.can_id == 0x18)
    {
      helloImHereStartupBreaklight = myMessage.data[0]; 
      if (helloImHereStartupBreaklight == global::sant)
      {
        bitWrite(acmOk,7,1);
      }
      else
      {
        bitWrite(acmOk,7,0);
      }
    }
  }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CanSend() {      // Sende data
using namespace cansignal;

// CAN message 0x14 - pingAllAcm

if (millis() - acmOkTimer > 5000)
{
  myMessage.can_id = 0x19;  
  myMessage.can_dlc = 2; 
  myMessage.data[0] = pingAllAcm;
  myMessage.data[1] = acmOk;

  mcp2515.sendMessage(&myMessage);
  acmOkTimer = millis();
}


// CAN message 0x270 - Shutdown Active
  myMessage.can_id = 0x270;  
  myMessage.can_dlc = 1; 
  myMessage.data[0] = shutdownActive;

  mcp2515.sendMessage(&myMessage);



// CAN message 0x630 - Ambient and turbo air temp
  myMessage.can_id = 0x630;  
  myMessage.can_dlc = 2; 
  myMessage.data[0] = ambientAirTemp;
  myMessage.data[1] = turboDischargeAirTemp;

  mcp2515.sendMessage(&myMessage);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
