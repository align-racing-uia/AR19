#include <can.h>
#include <mcp2515.h>
#include "rotaryswitch.h"

////////////////////////////////////////
//For use in AR19 Dashboard ACM ////////
//Last Updated 180319 1011  ////////////
//By Marcus N. Løvdal //////////////////
////////////////////////////////////////
//Note: Buttons are used with //////////
//Pull-down-resistors //////////////////
////////////////////////////////////////



//New addition: rotary switch cpp file. Yet to be tested.

//To do:
//Implement CANbus-functionality
//Implement Launch-Active Check
//Upload and test
//Gjør object-orientert (IKKE en prioritet............)

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
// WERY, WERY, WERY, WERY IMPORTANT //
// All variables connected to canbus (mcp2515.h) are only temporary MUST be discussed in plenium and concluded BEFORE actual implementation //
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //



  //CANbus 
  struct can_frame canMSG1, canMSG2, incommingMSG; 
  MCP2515 mcp2515(7); //Selcts pin for CANbus. IMPORTANT: must be 7 for ACM rev c

  //Pin number setup
  const int iButtonPins[4] = {2, 4, 7, 8}; //?Add ocrrect button pin numbers - In order: Left button, Right Button, Left Flappy, Right Flappy

  //Initialize variables
  bool bButtonValues[4] = {0,0,0,0}; //lb, rb, lf, rf (In order: Left button, Right button, Left flappy, Right flappy)

  //Misc. Constants
  const long lReleaseWithin = 200; //(ms). Must be <= 500. ?Add better time
  const bool prototyping = 1; //!!! ??? prototyping mode

  //Misc. Variables
  bool bBothButtonsPressed = 0;
  bool bLaunchActive = 0; //?Current implementation will not reset this value
  long lButtonReleaseTimeStamp = -1; //? -1 means the TimeStamp is ready to recieve a new value. The timestamp will not be updated as defined in the function MakeTimeStamp();
  uint32_t defaultMessageLenght = 4; //? default CANbus message lenght TBD




void setup() {
  //Initialize pins ///

  InitiateButtonPins();
  InitiateRotarySwitchPin();

  //CANbus ///
  canMSG1.can_id = 0xFF; //? id TBD
  canMSG2.can_id = 0xFE; //? id TBD

  canMSG1.can_dlc = defaultMessageLenght; //? message length TBD
  canMSG2.can_dlc = defaultMessageLenght; //? message length TBD

  for (int i = 0; i < 4; i++)
  {
    canMSG1.data[i] = 0;
    canMSG2.data[i] = 0;
  }

  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  //Potmeter///

  CalculateIntervals();

  //Other ///
  
  Serial.begin(9600); //for printing only
}


//Main////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
void loop() {
  Serial.print("1 \n");
  CheckButtons();
  CheckRotarySwitch();
  SendRotarySwitchData();

  //Button functionality///////////////////////////////////////////////////////////////////////////////////////////////////
  if ( IsBothButtonsPressed() && !bLaunchActive) //if both buttons are pressed AND Launch is currently not active/regulating, check if both buttons are held
  {
        //?Send CAN-message to LB that both buttons are pressed, ?confirm that this message has been recieved. ???TBD!!!!
        canMSG1.data[0] = 1;
        mcp2515.sendMessage(&canMSG1);
        mcp2515.readMessage(&incommingMSG);
        if (mcp2515.readMessage(&incommingMSG) == MCP2515::ERROR_OK) //If a new message is awailable
        {
          if ((incommingMSG.can_id == 0x00) && (incommingMSG.data[0] = 0xFF)) //Check if the current message has the correct ID and the desiret data
          {
          bBothButtonsPressed = 1;
          bLaunchActive = 1; //?if and ONLY if confirmed with LB
        
          Serial.print("Launch Active Confirmed \n");
          }
        }
          
        if (prototyping)
        {
          bBothButtonsPressed = 1;
          bLaunchActive = 1;
        }
        
        Serial.print("2 \n");
  }
  else if (bBothButtonsPressed && bLaunchActive)  //if both buttons are confirmed held and Launch is currently active,
  {
    Serial.print("3 \n");
    if ( IsOneButtonNotPressed() ) //if one of the buttons are released,
      {
      MakeTimeStamp();
      Serial.print("4 \n");
      if ( ButtonsReleasedWithinTime() && IsNoButtonsPressed() ) //If dt < 500ms AND both buttons are released
        {
        //Confirmed Take-off
        //Send CANbus-message to LB to confirm takeoff
        lButtonReleaseTimeStamp = -1;
        bLaunchActive = 0;
        Serial.print("Confirmed Take-off");
        bBothButtonsPressed = 0;
        delay(3000); //???
        }
      
      else if (!ButtonsReleasedWithinTime()) //If dt > 500ms
        {
        //Take-off Failure
        //?Send and confirm CANbus-message to Launch Board that the vehicle should revert to pre-launch SAFELY (considering ETC control)
        Serial.print("TAKEOFF FAILURE");
        lButtonReleaseTimeStamp = -1;
        bLaunchActive = 0;
        bBothButtonsPressed = 0;
        delay(3000); //???
        }
        
      }
    else //No buttons are released yet
      {
        //Do nothing
      }
   if ( IsBothButtonsPressed() ) //If dt < 500ms and both buttons are pressed
   {
     lButtonReleaseTimeStamp = -1;
     Serial.print("5 \n");
   }
      
  }

 //Flappies-functionality//////////////////////////////////////////////////////////////////////////////////////////////////////////






  Serial.print("\n");
  delay(100); //???
  
}



// Button/sensor inputs ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CheckButtons() 
  {
    for (int i = 0; i < 2; i++)
    {
      bButtonValues[i] = digitalRead(iButtonPins[i]);
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

//Pin initialisation (buttons) /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitiateButtonPins()
{
  for (int i = 0; i < 4; i++)
  {
  pinMode(iButtonPins[i], INPUT);
  }
}




//Usefull lines for debugging
//
//Serial.print(bButtonValues[0]); Serial.print(" "); Serial.print(bButtonValues[1]); Serial.print("\n"); Prints the left and right current button state
//Serial.print("1 \n"); Prints a number and newline, usefull for tracking the flow
