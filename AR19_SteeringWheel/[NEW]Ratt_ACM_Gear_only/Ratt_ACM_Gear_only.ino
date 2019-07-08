#include <SPI.h>
#include <mcp2515.h>

///////////////////////////////////
//For use in AR19 Dashboard ACM ///
//Last Updated 040719       ///////
//By Henrik Brådland  /////////////
///////////////////////////////////



  //CANbus 
  struct can_frame debuggingMSG, flappyMSG, incommingMSG, rattACMonline, neutrallMSG;


  
  MCP2515 mcp2515(7); //Selcts pin for CANbus. IMPORTANT: must be 7 for ACM rev c


  //Pin number setup
  const int iButtonPins[4] = {A2, A3, A4, A5}; // In order: Left button, Right Button, Left Flappy, Right Flappy

  //Initialize variables
  bool bButtonValues[4] = {0,0,0,0}; //lb, rb, lf, rf (In order: Left button, Right button, Left flappy, Right flappy)

  //Misc. Constants
  const long lReleaseWithin = 200; //(ms). Must be <= 500
  const long minButtonBounceDeltaTime = 10; //(ms)

  //Misc. Variables
  bool bCANbusMessageOK = 0;
  long lButtonReleaseTimeStamp = -1; //? -1 means the TimeStamp is ready to recieve a new value. The timestamp will not be updated as defined in the function MakeTimeStamp();
  long buttonTimeStamp = 0;

  

  
  uint8_t messagesRecieved = 0;
  long timeStamp = 0;
  long rotaryTimeStamp = 0;
  uint8_t IncMessageLocal[8];
  uint16_t IncMessageID = 0x6FF;
  
  uint8_t currentSystemState = 0;

  uint8_t previousLeftFlappyPosition = 0;
  uint8_t previousRightFlappyPosition = 0;
  uint8_t previousLeftButtonPosition = 0;
  uint8_t previousRightButtonPosition = 0;
  

  const uint8_t globalTrue = 0xF0;
  const uint8_t globalFalse = 0x0F;

  

void setup() 
{


  InitiateButtonPins();




  //CANbus ///
  

  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();
  
  debuggingMSG.can_id       = 0x1FF;
  flappyMSG.can_id          = 0x240; 
  rattACMonline.can_id      = 0x015;
  neutrallMSG.can_id        = 0x042;
  
  debuggingMSG.can_dlc      = 5;
  flappyMSG.can_dlc         = 2;
  rattACMonline.can_dlc     = 1;
  neutrallMSG.can_dlc       = 2;



  for (uint8_t i = 0; i < debuggingMSG.can_dlc; i++)
  {
    debuggingMSG.data[i] = 0;
  }

  rattACMonline.data[0] = 0xF0;
  mcp2515.sendMessage(&rattACMonline);


  //Other //
  
}


//Main///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop() 
{
  CheckButtons();
  UppdateButtons();
  readCANbus();
}



// Buttons/Flappies ///////////////////////////////////////////////////////////////////


void UppdateButtons()
{ 

  if (bButtonValues[0] == 1 && previousLeftButtonPosition == 0) 
  { 
    SetLEDColor('w');
    mcp2515.sendMessage(&neutrallMSG);
    previousLeftButtonPosition = 1;
  }
  else if (bButtonValues[0] == 0 && previousLeftButtonPosition == 1)
  {
    flappyMSG.data[1] = 0x0F;
    previousLeftButtonPosition = 0;
  }


  if (bButtonValues[1] == 1 && previousRightButtonPosition == 0) 
  {
    SetLEDColor('w');
    mcp2515.sendMessage(&neutrallMSG);
    previousRightButtonPosition = 1;
  }
  else if (bButtonValues[1] == 0 && previousRightButtonPosition == 1) 
  {
    previousRightButtonPosition = 0;
  }


  if (bButtonValues[2] == 1 && previousLeftFlappyPosition == 0) //if left flappy is pulled (opp-flank)
  {
    SetLEDColor('w');
    flappyMSG.data[0] = 0xF0;
    mcp2515.sendMessage(&flappyMSG);
    previousLeftFlappyPosition = 1;
  }
  else if (bButtonValues[2] == 0 && previousLeftFlappyPosition == 1) //if left flappy is released (ned-flank)
  {
    flappyMSG.data[0] = 0x0F;
    previousLeftFlappyPosition = 0;
  }

  
  if (bButtonValues[3] == 1 && previousRightFlappyPosition == 0)  //if right flappy is pulled (opp-flank)
  {
    SetLEDColor('w');
    flappyMSG.data[1] = 0xF0;
    mcp2515.sendMessage(&flappyMSG);
    previousRightFlappyPosition = 1;
  }
  else if (bButtonValues[3] == 0 && previousRightFlappyPosition == 1) //if right fappy is released (ned-flank)
  {
    flappyMSG.data[1] = 0x0F;
    previousRightFlappyPosition = 0;
  }

  
}
  

// Button/sensor inputs ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CheckButtons() 
  {
    if ( millis() - buttonTimeStamp > minButtonBounceDeltaTime) 
    {
      for (int i = 0; i < 4; i++)
      {
        bButtonValues[i] = !digitalRead(iButtonPins[i]);
      }
      buttonTimeStamp = millis();
    }
  }






//CANbus////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void readCANbus()
{

    for (int i = 0; i < 8; i++)
    {
      IncMessageLocal[i] = 0;
    }
    IncMessageID = 0x6FF; 
    
    
  if (mcp2515.readMessage(&incommingMSG) == MCP2515::ERROR_OK)
  {
    for (int i = 0; i < incommingMSG.can_dlc; i++)
    {
         IncMessageLocal[i] = incommingMSG.data[i];
    }
    IncMessageID = incommingMSG.can_id;

    if (IncMessageID == 0x019 && IncMessageLocal[0] == 0xF0)
    { 
        mcp2515.sendMessage(&rattACMonline);
    }


    
  }
  else
    {
      bCANbusMessageOK = 0;
    }
  

}



//Mics. Functions///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MakeTimeStamp() //Makes a timestamp if the current timestamp is ready for update
{
  if (lButtonReleaseTimeStamp == -1)
  {
    lButtonReleaseTimeStamp = millis();
  }
  else
  {
    //?Do nothing
  }
}

bool ButtonsReleasedWithinTime() //Returns true if dt < 500ms, false else
{
  return (millis() - lButtonReleaseTimeStamp) < lReleaseWithin;
}




//LED Colors ///////////////////////////////////////////////////////////////////////////////


void SetLEDColor(char color)
{
  analogWrite(5, 255);
  analogWrite(6, 255);
  analogWrite(9, 255);
  
  switch(color)
  {
     case 'r': //Red
      analogWrite(5, 0);
      break;
      
    case 'g': //Green
      analogWrite(6, 0);
      break;

    case 'b': //Blue
      analogWrite(9, 0);
      break;

    case 'y': //Yellow
      analogWrite(5, 0);
      analogWrite(6, 0);
      break;

    case 'c': //Cyan
      analogWrite(6, 0);
      analogWrite(9, 0);
      break;

    case 'm': //Magenta
      analogWrite(5, 0);
      analogWrite(9, 0);
      break;

    case 'w': //White
      analogWrite(5, 0);
      analogWrite(6, 0);
      analogWrite(9, 0);
      break;

    default:
      //Do thing, invalid color or turns LED off
      break;
    
  }
}



//Pin initialisation (buttons) /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitiateButtonPins()
{
  for (int i = 0; i < 4; i++) //4 ink. flappies 
  {
  pinMode(iButtonPins[i], INPUT_PULLUP);
  }
}



//Usefull lines for debugging
//
//Serial.print(bButtonValues[0]); Serial.print(" "); Serial.print(bButtonValues[1]); Serial.print("\n"); Prints the left and right current button state
//Serial.print("1 \n"); Prints a number and newline, usefull for tracking the flow
