void CanRecieve() 
{
  if (mcp2515.readMessage(&myMessage) == MCP2515::ERROR_OK) 
  {
    // CAN message 0x42 - Neutral signal and clutch override.
    if (myMessage.can_id == 0x42)
    {
      cansignal::neutralSignal = myMessage.data[0];
      cansignal::clutchOverride =  myMessage.data[1];
      myMessage.can_id = 0x485;  
      myMessage.can_dlc = 1; 
      myMessage.data[0] = global::sant;
      mcp2515.sendMessage(&myMessage); 
    }
    else
    {
      cansignal::neutralSignal = global::tull;
      cansignal::clutchOverride = global::tull;  
    }
  
  
    
    // CAN message 0x240 - Gear paddles.
    if (myMessage.can_id == 0x240)
    {
      cansignal::gearDownSignal  = myMessage.data[0];
      cansignal::gearUpSignal    = myMessage.data[1]; 
      myMessage.can_id = 0x485;  
      myMessage.can_dlc = 1; 
      myMessage.data[0] = global::sant;
      mcp2515.sendMessage(&myMessage); 
    }
    else
    {
      cansignal::gearDownSignal = global::tull;
      cansignal::gearUpSignal = global::tull;    
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





void CanSend() // Sende data
{      


// CAN message 0x21 - primary messages

  if (millis() - cansignal::primaryTimer > 20)
  
  {
    myMessage.can_id = 0x21;  
    myMessage.can_dlc = 2; 
    myMessage.data[0] = gearposition::currentGear;
    myMessage.data[1] = clutchpressure::InBar; 
    
    mcp2515.sendMessage(&myMessage);
    cansignal::primaryTimer = millis();
  }

// CAN message 0x480 - Error states to telemetry.

  if((cansignal::clutchPressureError == global::sant || cansignal::gearPositionError == global::sant || cansignal::gearChangeFailed == global::sant || cansignal::neutralFailed == global::sant) && (millis()-cansignal::telemetryTimer > 20))
  {
    myMessage.can_id  = 0x480;  
    myMessage.can_dlc = 4; 
    myMessage.data[0] = cansignal::clutchPressureError;         //Errorstate - Clutch Pressure out of range
    myMessage.data[1] = cansignal::gearPositionError;           //Sends telemetry signal if gear position sensor is invalid
    myMessage.data[2] = cansignal::gearChangeFailed;            //The desired gear was not reached within the given period
    myMessage.data[3] = cansignal::neutralFailed;               //sens error message that neutral attempt failed //Henrik: ID'en ligger ikke i registeret
  
    mcp2515.sendMessage(&myMessage);
    cansignal::telemetryTimer = millis();
  }

    // CAN message 0x1 - Activate shutdown circuit
    
  if(cansignal::clutchPressureError == global::sant || cansignal::gearPositionError == global::sant)
  {
    myMessage.can_id  = 0x1;  
    myMessage.can_dlc = 17; 
    myMessage.data[0] = global::sant; // Activates the shutdown circuit

    mcp2515.sendMessage(&myMessage);
  }
}
