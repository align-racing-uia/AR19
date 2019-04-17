void CanSetup() // Sets up the CAN-Bus protocol.  
{
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS);
  mcp2515.setNormalMode();


  mcp2515.sendMessage(&myMessage);

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void CanRecieve() {

using namespace cansignal;
//Ta imot data

 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) {
  
    if (myMessage.can_id == 0x410 && myMessage.can_dlc == 1)//Her må riktig CAN-id leggast inn. 
    {
      helloImHereStartupPedal = myMessage.data[0]; 
      if (helloImHereStartupPedal == global::sant)
      {
        bitWrite(acmOk,0,1);
      }
      else
      {
        bitWrite(acmOk,0,0);
      }
    }



    else if (myMessage.can_id == 0x410)//Her må riktig CAN-id leggast inn. 
    {
      helloImHereStartupHallFL = myMessage.data[0]; 
      if (helloImHereStartupHallFL == global::sant)
      {
        bitWrite(acmOk,1,1);
      }
      else
      {
        bitWrite(acmOk,1,0);
      }
    }


    else if (myMessage.can_id == 0x410)//Her må riktig CAN-id leggast inn. 
    {
      helloImHereStartupHallFR = myMessage.data[0]; 
      if (helloImHereStartupHallFR == global::sant)
      {
        bitWrite(acmOk,2,1);
      }
      else
      {
        bitWrite(acmOk,2,0);
      }
    }


    else if (myMessage.can_id == 0x410)//Her må riktig CAN-id leggast inn. 
    {
      helloImHereStartupHALLR = myMessage.data[0]; 
      if (helloImHereStartupHALLR == global::sant)
      {
        bitWrite(acmOk,3,1);
      }
      else
      {
        bitWrite(acmOk,3,0);
      }
    }


    else if (myMessage.can_id == 0x410)//Her må riktig CAN-id leggast inn. 
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

    else if (myMessage.can_id == 0x410)//Her må riktig CAN-id leggast inn. 
    {
      helloImHereStartupDash = myMessage.data[0]; 
      if (helloImHereStartupDash == global::sant)
      {
        bitWrite(acmOk,5,1);
      }
      else
      {
        bitWrite(acmOk,5,0);
      }
    }


    else if (myMessage.can_id == 0x410)//Her må riktig CAN-id leggast inn. 
    {
      helloImHereStartupETB = myMessage.data[0]; 
      if (helloImHereStartupETB == global::sant)
      {
        bitWrite(acmOk,6,1);
      }
      else
      {
        bitWrite(acmOk,6,0);
      }
    }

    else if (myMessage.can_id == 0x410)//Her må riktig CAN-id leggast inn. 
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


// Her skal den ta i mot ACM_OK signal fra alle ACM'er
/*
 if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) {
  
    myMessage.can_id = 0x410;  // ID'en til meldingen som er mottat
    myMessage.can_dlc = 3; // Antal byte med data i meldingen 1 til 8
    myMessage.data[0] = breakPressure1; // Det første bytet med data
  }

*/

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CanSend() {      // Sende data
using namespace cansignal;

// CAN message 0x14 - pingAllAcm

if (pingAllAcm == global::sant)
{
  myMessage.can_id = 0x14;  
  myMessage.can_dlc = 1; 
  myMessage.data[0] = pingAllAcm;

  mcp2515.sendMessage(&myMessage);


  pingAllAcm == global::tull;
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
