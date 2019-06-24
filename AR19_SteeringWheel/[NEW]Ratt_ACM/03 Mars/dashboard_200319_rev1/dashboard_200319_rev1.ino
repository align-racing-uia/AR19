#include <SPI.h>
#include <mcp2515.h>
#include "rotaryswitch.h"

////////////////////////////////////////
//For use in AR19 Dashboard ACM ////////
//Last Updated 190319 1902  ////////////
//By Marcus N. Løvdal //////////////////
////////////////////////////////////////
//Note: Buttons are used with //////////
//Pull-down-resistors //////////////////
////////////////////////////////////////



//New addition: rotary switch cpp file, tested on normal conditions. Added bool "prototyping", which is only to be activated under testing. Implemented out-of-range detection for rotary potmeter. 

//To do:
//Implement safety check (button press time-out, out of range signals)
//Implement CANbus-functionality
//Implement Launch-Active Check
//Upload and test
//Gjør object-orientert (IKKE en prioritet............)

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
// VERY, VERY, VERY, VERY IMPORTANT //
// All variables connected to canbus (mcp2515.h) are only temporary MUST be discussed in plenium and concluded BEFORE actual implementation //
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //


  //CANbus 
  struct can_frame canMSG1; 
  struct can_frame canMSG2;
  struct can_frame incommingMSG;
  struct can_frame outgoingMSG;
  struct can_frame debuggingMSG;
  MCP2515 mcp2515(7); //Selcts pin for CANbus. IMPORTANT: must be 7 for ACM rev c


  //Pin number setup
  const int iButtonPins[4] = {A4, A5, 0, 0}; //?Add ocrrect button pin numbers - In order: Left button, Right Button, Left Flappy, Right Flappy

  //Initialize variables
  bool bButtonValues[4] = {0,0,0,0}; //lb, rb, lf, rf (In order: Left button, Right button, Left flappy, Right flappy)

  //Misc. Constants
  const long lReleaseWithin = 200; //(ms). Must be <= 500. ?Add better time
  const bool prototyping = 0; //!!! ??? prototyping mode, only to be active under testing. 

  //Misc. Variables
  bool bBothButtonsPressed = 0;
  bool bLaunchActive = 0; //?Current implementation will not reset this value
  long lButtonReleaseTimeStamp = -1; //? -1 means the TimeStamp is ready to recieve a new value. The timestamp will not be updated as defined in the function MakeTimeStamp();

  uint8_t messagesRecieved = 0;




void setup() {
  //Initialize pins ///


  InitiateButtonPins();
  InitiateRotarySwitchPin();




  //CANbus ///
  //List of messages:
  

  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  
  canMSG1.can_id = 0xF0; //? id TBD
  canMSG2.can_id = 0xFE; //? id TBD
  debuggingMSG.can_id = 0xFF;

  canMSG1.can_dlc = 4; //? message length TBD
  canMSG2.can_dlc = 4; //? message length TBD
  debuggingMSG.can_dlc = 4;

  debuggingMSG.data[0] = 0;
  debuggingMSG.data[1] = 0;
  debuggingMSG.data[2] = 0;
  debuggingMSG.data[3] = 0;

  

  for (int i = 0; i < 4; i++)
  {
    canMSG1.data[i] = 0;
    canMSG2.data[i] = 0;
  }



  //Potmeter///

  CalculateIntervals();

  //Other ///
  
  Serial.begin(9600); //for printing only
//  analogWrite(5,0);
  
}


//Main////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
void loop() {
  Serial.print("1 \n");
  CheckButtons();
  CheckRotarySwitch();
  ActualPotmeterPosition();
  SendRotarySwitchData();

  mcp2515.sendMessage(&debuggingMSG);
  debuggingMSG.data[0] = 0;






  //Button functionality///////////////////////////////////////////////////////////////////////////////////////////////////
  if ( IsBothButtonsPressed() && !bLaunchActive) //if both buttons are pressed AND Launch is currently not active/regulating, check if both buttons are held
  {
        analogWrite(6, 0);
        debuggingMSG.data[0] = 1;
        //?Send CAN-message to LB that both buttons are pressed, ?confirm that this message has been recieved. ???TBD!!!!
        canMSG1.data[0] = 1;
        mcp2515.sendMessage(&canMSG1);
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
        
        analogWrite(6, 0);
        analogWrite(5, 0);
        
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
                analogWrite(5, 255);
                analogWrite(6, 255);
                analogWrite(9, 0);
        bBothButtonsPressed = 0;
        delay(3000); //???
        analogWrite(9, 255);
        }
      
      else if (!ButtonsReleasedWithinTime()) //If dt > 500ms
        {
        //Take-off Failure
        //?Send and confirm CANbus-message to Launch Board that the vehicle should revert to pre-launch SAFELY (considering ETC control)
        Serial.print("TAKEOFF FAILURE");
        
        analogWrite(6, 255);
        lButtonReleaseTimeStamp = -1;
        bLaunchActive = 0;
        bBothButtonsPressed = 0;
        delay(3000); //???
        analogWrite(5, 255);
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


 // //////////////
 
  analogWrite(6, 255);
  Serial.print("\n");
 // delay(100); //???

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

void SendRotarySwitchData()
{
  outgoingMSG.can_id = 0x01;
  outgoingMSG.can_dlc = 4;
  outgoingMSG.data[0] = bButtonValues[0];
  outgoingMSG.data[1] = bButtonValues[1];
  outgoingMSG.data[2] = return8bitRawPotmeterValue();
  outgoingMSG.data[3] = ActualPotmeterPosition();
  mcp2515.sendMessage(&outgoingMSG);
  
}







//Pin initialisation (buttons) /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitiateButtonPins()
{
  for (int i = 0; i < 2; i++) //4 ink. flappies, flappies are currently not 
  {
  pinMode(iButtonPins[i], INPUT);
  }
}



//Usefull lines for debugging
//
//Serial.print(bButtonValues[0]); Serial.print(" "); Serial.print(bButtonValues[1]); Serial.print("\n"); Prints the left and right current button state
//Serial.print("1 \n"); Prints a number and newline, usefull for tracking the flow
