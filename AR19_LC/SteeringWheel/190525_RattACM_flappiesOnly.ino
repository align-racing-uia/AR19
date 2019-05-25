#include <SPI.h>
#include <mcp2515.h>
#include "rotaryswitch.h"

///////////////////////////////////
//For use in AR19 Dashboard ACM ///
//Last Updated 250519       ///////
//By Marcus N. Løvdal /////////////
///////////////////////////////////

// This version does not include launch buttons or rotary potmeter, it is a temporary version to be used for testing of vehicle.
// This version is used on ACM with pull-down resistors soldered post-production
// The rotary switch value will always be set to zero


  //CANbus 
  struct can_frame launchButtonsMSG; 
  struct can_frame debuggingMSG;
  struct can_frame flappyMSG;
  struct can_frame potmeterMSG;
  struct can_frame incommingMSG;
  struct can_frame rattACMonline;


  
  MCP2515 mcp2515(7); //Selcts pin for CANbus. IMPORTANT: must be 7 for ACM rev c


  //Pin number setup
  const int iButtonPins[4] = {0, 0, A4, A5}; // In order: Left button, Right Button, Left Flappy, Right Flappy

  //Initialize variables
  bool bButtonValues[4] = {0,0,0,0}; //lb, rb, lf, rf (In order: Left button, Right button, Left flappy, Right flappy)

  //Misc. Constants
  const long lReleaseWithin = 200; //(ms). Must be <= 500
  const long minButtonBounceDeltaTime = 10; //(ms)

  //Misc. Variables
  bool bLaunchActive = 0; //?Current implementation will not reset this value
  bool bCANbusMessageOK = 0;
  long lButtonReleaseTimeStamp = -1; //? -1 means the TimeStamp is ready to recieve a new value. The timestamp will not be updated as defined in the function MakeTimeStamp();
  long buttonTimeStamp = 0;

  

  
  uint8_t messagesRecieved = 0;
  long timeStamp = 0;
  long rotaryTimeStamp = 0;
  uint8_t IncMessageLocal[8];
  uint16_t IncMessageID = 0x6FF;
  
  uint8_t previousSentPotmeterPosition = 0;
  uint8_t currentSystemState = 0;

  uint8_t previousLeftFlappyPosition = 0;
  uint8_t previousRightFlappyPosition = 0;

  

void setup() {
  //Initialize pins ///


  InitiateButtonPins();
  InitiateRotarySwitchPin();




  //CANbus ///
  //List of messages:
  

  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS);
  mcp2515.setNormalMode();
  
  launchButtonsMSG.can_id   = 0x250; 
  debuggingMSG.can_id       = 0x1FF;
  flappyMSG.can_id          = 0x240; 
  potmeterMSG.can_id        = 0x235;
  rattACMonline.can_id      = 0x015;
  
  launchButtonsMSG.can_dlc  = 1; //? message length TBD
  debuggingMSG.can_dlc      = 5;
  flappyMSG.can_dlc         = 1;
  potmeterMSG.can_dlc       = 1;
  rattACMonline.can_dlc     = 1;

  for (uint8_t i = 0; i < debuggingMSG.can_dlc; i++)
  {
    debuggingMSG.data[i] = 0;
  }

  for (uint8_t i = 0; i < launchButtonsMSG.can_dlc; i++)
  {
    launchButtonsMSG.data[i] = 0;
  }

  rattACMonline.data[0] = 0;
  mcp2515.sendMessage(&rattACMonline);

  //Potmeter///

  CalculateIntervals();

  CheckRotarySwitch();
  ActualPotmeterPosition();
  SendRotarySwitchData(ActualPotmeterPosition());

  //Other //
  
}


//Main///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop() {
  CheckButtons();
  UpdateFlappies();
  CheckRotarySwitch();
  ActualPotmeterPosition();
  SendRotarySwitchData(ActualPotmeterPosition());
  readCANbus();
  UpdateSystemState();

//  for (uint8_t i = 0; i < 3; i++)
//  {
//    debuggingMSG.data[i] = bButtonValues[i];
//  }
//  debuggingMSG.data[4] = currentSystemState;

//  mcp2515.sendMessage(&debuggingMSG);


  //Button functionality///////////////////////////////////////////////////////////////////////////////////////////////////
  if ( IsBothButtonsPressed() && !bLaunchActive) //if both buttons are pressed AND Launch is currently not active/regulating, check if both buttons are held
  {
     SetLEDColor('g'); //Green
//        debuggingMSG.data[0] = 1;
        
        
        if( millis() - timeStamp > 500)
        {
        launchButtonsMSG.data[0] = 0xF0;
        mcp2515.sendMessage(&launchButtonsMSG);
        timeStamp = millis();
        }

        
        if (currentSystemState == 20) //Check if the current message has the correct ID and the desiret data
          {
            bLaunchActive = 1; //?if and ONLY if confirmed with LB
          }

       
  }
  if (bLaunchActive)  //if both buttons are confirmed held and Launch is currently active,
  {


        if (currentSystemState == 20)
        {
          SetLEDColor('y'); //yellow
        }
        else if (currentSystemState == 30)
        {
          SetLEDColor('c'); //cyan
        }
        else
        {
          //Do nothing
        } 

        
    if ( IsOneButtonNotPressed() ) //if one of the buttons are released,
      {
      MakeTimeStamp();
      if ( ButtonsReleasedWithinTime() && IsNoButtonsPressed() && (currentSystemState == 30) ) //If dt < 500ms AND both buttons are released AND the vehicle is ready to launch
        {
        //Confirmed Take-off
        lButtonReleaseTimeStamp = -1;
        bLaunchActive = 0;
        SetLEDColor('b'); //Blue

                
        launchButtonsMSG.data[0] = 0x0F;
        
        delay(1000); //???
        
        }
      
      else if (!ButtonsReleasedWithinTime()) //If dt > 500ms
        {
        //Take-off Failure
        //?Send and confirm CANbus-message to Launch Board that the vehicle should revert to pre-launch SAFELY (considering ETC control)

        lButtonReleaseTimeStamp = -1;
        bLaunchActive = 0;

        launchButtonsMSG.data[0] = 0xAA;
        
        mcp2515.sendMessage(&launchButtonsMSG);
        
        SetLEDColor('r'); //Red
        delay(1000);
        }
        
      }

   if ( IsBothButtonsPressed() ) //If dt < 500ms and both buttons are pressed
   {
     lButtonReleaseTimeStamp = -1;
   }
      
  }

  
  if((!IsBothButtonsPressed() && bLaunchActive) || (!IsBothButtonsPressed() && !bLaunchActive))
  {
    SetLEDColor(0); //LED Off
    
    if ( millis() - timeStamp > 1000)
    {
      launchButtonsMSG.data[0] = 0x55;
      mcp2515.sendMessage(&launchButtonsMSG);
      timeStamp = millis();
    }
  }



}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Update System State /////////////////////////////////////////////////////////

void UpdateSystemState()
{
  if (IncMessageID == 0x2A0)
  {
    currentSystemState = IncMessageLocal[0];
  }
  else
  {
    //do nothing
  }
}


// Flappies ///////////////////////////////////////////////////////////////////


void UpdateFlappies()
{ 


  if (bButtonValues[2] == 1 && previousLeftFlappyPosition == 0) //if left flappy is pulled (opp-flank)
  {
    flappyMSG.data[0] = 1;
    mcp2515.sendMessage(&flappyMSG);
    previousLeftFlappyPosition = 1;
  }
  else if (bButtonValues[2] == 0 && previousLeftFlappyPosition == 1) //if left flappy is released (ned-flank)
  {
    flappyMSG.data[0] = 0xAA;
    previousLeftFlappyPosition = 0;
  }

  
  if (bButtonValues[3] == 1 && previousRightFlappyPosition == 0)  //if right flappy is pulled (opp-flank)
  {
    flappyMSG.data[0] = 0xFF;
    mcp2515.sendMessage(&flappyMSG);
    previousRightFlappyPosition = 1;
  }
  else if (bButtonValues[3] == 0 && previousRightFlappyPosition == 1) //if right fappy is released (ned-flank)
  {
    flappyMSG.data[0] = 0xAA;
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
        bButtonValues[i] = digitalRead(iButtonPins[i]);
      }
      buttonTimeStamp = millis();
    }
  }

//Conditions/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsBothButtonsPressed() //Returns true if both buttons are pressed (AND)
{
  return bButtonValues[0] && bButtonValues[1];
}

bool IsOneButtonNotPressed() //Returns true if one or more of the buttons are NOT pressed  (!AND)
{
  return !bButtonValues[0] || !bButtonValues[1];
}

bool IsNoButtonsPressed() //Returns true if NO buttons are pressed (!OR)
{
  return !bButtonValues[0] && !bButtonValues[1];
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





void SendRotarySwitchData(uint8_t dataInput)
{
  if (dataInput != previousSentPotmeterPosition)
  {
    potmeterMSG.data[0] = dataInput;
    mcp2515.sendMessage(&potmeterMSG);
    previousSentPotmeterPosition = dataInput;
  }
  else
  {
    //Do nothing
  }
  

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
  pinMode(iButtonPins[i], INPUT);
  }
}



//Usefull lines for debugging
//
//Serial.print(bButtonValues[0]); Serial.print(" "); Serial.print(bButtonValues[1]); Serial.print("\n"); Prints the left and right current button state
//Serial.print("1 \n"); Prints a number and newline, usefull for tracking the flow
