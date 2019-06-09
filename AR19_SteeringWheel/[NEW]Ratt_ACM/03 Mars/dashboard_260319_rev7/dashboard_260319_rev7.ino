#include <SPI.h>
#include <mcp2515.h>
#include "rotaryswitch.h"

///////////////////////////////////
//For use in AR19 Dashboard ACM ///
//Last Updated 260319 1112  ///////
//By Marcus N. Løvdal /////////////
///////////////////////////////////
//Note: Buttons are used with /////
//Pull-down-resistors /////////////
///////////////////////////////////



//New addition: list of common failures 

//To do:
//Implement safety check (button press time-out, out of range signals)
//Implement CANbus-functionality
//Implement Launch-Active Check
//Upload and test
//Gjør object-orientert (IKKE en prioritet............)
//add functionality such that MSG1 is sent maximum once every second

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
  struct can_frame testMSG; 
  
  MCP2515 mcp2515(7); //Selcts pin for CANbus. IMPORTANT: must be 7 for ACM rev c


  //Pin number setup
  const int iButtonPins[4] = {A4, A5, 0, 0}; //?Add ocrrect button pin numbers - In order: Left button, Right Button, Left Flappy, Right Flappy

  //Initialize variables
  bool bButtonValues[4] = {0,0,0,0}; //lb, rb, lf, rf (In order: Left button, Right button, Left flappy, Right flappy)

  //Misc. Constants
  const long lReleaseWithin = 200; //(ms). Must be <= 500. ?Add better time
  const bool prototyping = 0; //!!! ??? prototyping mode, only to be active under testing. 

  //Misc. Variables
 // bool bBothButtonsPressed = 0;
  bool bLaunchActive = 0; //?Current implementation will not reset this value
  bool bCANbusMessageOK = 0;
  long lButtonReleaseTimeStamp = -1; //? -1 means the TimeStamp is ready to recieve a new value. The timestamp will not be updated as defined in the function MakeTimeStamp();

  uint8_t messagesRecieved = 0;
  long timeStamp = 0;
  long rotaryTimeStamp = 0;
  uint8_t IncMessageLocal[8];
  uint16_t IncMessageID = 0x6FF;
  






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
  
  canMSG1.can_id = 0xFF; //? id TBD, confirmation request from dash to LB
  canMSG2.can_id = 0xFE; //? id TBD
  debuggingMSG.can_id = 0x1FF;
  testMSG.can_id = 0xDD;

  canMSG1.can_dlc = 4; //? message length TBD
  canMSG2.can_dlc = 4; //? message length TBD
  debuggingMSG.can_dlc = 4;
  testMSG.can_dlc = 1;

  debuggingMSG.data[0] = 0;
  debuggingMSG.data[1] = 0;
  debuggingMSG.data[2] = 0;
  debuggingMSG.data[3] = 0;
  
  testMSG.data[0] = 0x00;
  
  canMSG1.data[1] = 0x00;  

  for (int i = 0; i < 4; i++)
  {
    canMSG1.data[i] = 0;
    canMSG2.data[i] = 0;
  }



  //Potmeter///

  CalculateIntervals();

  //Other ///
  
//  analogWrite(5,0);
  
}


//Main////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
void loop() {
  CheckButtons();
  CheckRotarySwitch();
  ActualPotmeterPosition();
  SendRotarySwitchData();
  
  readCANbus();
  debuggingMSG.data[0] = 0;
//  mcp2515.sendMessage(&debuggingMSG);

  //Button functionality///////////////////////////////////////////////////////////////////////////////////////////////////
  if ( IsBothButtonsPressed() && !bLaunchActive) //if both buttons are pressed AND Launch is currently not active/regulating, check if both buttons are held
  {
     SetLEDColor('g');
/*        analogWrite(6, 0);
        analogWrite(5, 255);
        analogWrite(9, 255);  */
        debuggingMSG.data[0] = 1;
        
        
        if( millis() - timeStamp > 500)
        {
        canMSG1.data[0] = 0xF0;
        mcp2515.sendMessage(&canMSG1);
        timeStamp = millis();
        }

        
        if ((IncMessageID == 0x5C1) && (IncMessageLocal[0] == 2)) //Check if the current message has the correct ID and the desiret data
          {

        
       //   bBothButtonsPressed = 1;
          bLaunchActive = 1; //?if and ONLY if confirmed with LB
        
 //         Serial.print("Launch Active Confirmed \n");
          }
          
        if (prototyping)
        {
 //         bBothButtonsPressed = 1;
          bLaunchActive = 1;
        }
        

  }
  if (bLaunchActive)  //if both buttons are confirmed held and Launch is currently active,
  {



        if ( IncMessageLocal[0] == 2  )
        {
          SetLEDColor('y');
/*                  analogWrite(6, 0);
                  analogWrite(5, 0);
                  analogWrite(9, 255);  */
        }
   else if ( IncMessageLocal[0] == 3 )
        {
          SetLEDColor('c');
/*                  analogWrite(6, 0);
                  analogWrite(5, 255);
                  analogWrite(9, 0);  */
        }
        else
        {
          
        } 

        
    if ( IsOneButtonNotPressed() ) //if one of the buttons are released,
      {
        mcp2515.sendMessage(&testMSG);
      MakeTimeStamp();
      if ( ButtonsReleasedWithinTime() && IsNoButtonsPressed() ) //If dt < 500ms AND both buttons are released
        {
        //Confirmed Take-off
        //Send CANbus-message to LB to confirm takeoff
        lButtonReleaseTimeStamp = -1;
        bLaunchActive = 0;
        SetLEDColor('b');
 /*               analogWrite(5, 255);
                analogWrite(6, 255);
                analogWrite(9, 0);  */

                
        canMSG1.data[0] = 0x0F;
        canMSG1.data[1] = 0xFF;
        mcp2515.sendMessage(&canMSG1);
 //       mcp2515.sendMessage(&canMSG1);
 //       mcp2515.sendMessage(&canMSG1);
   //     bBothButtonsPressed = 0;
        
        delay(3000); //???
        SetLEDColor(0);
        
/*        analogWrite(9, 255);
        analogWrite(6, 255);
        analogWrite(5, 255);  */
        
        }
      
      else if (!ButtonsReleasedWithinTime()) //If dt > 500ms
        {
        //Take-off Failure
        //?Send and confirm CANbus-message to Launch Board that the vehicle should revert to pre-launch SAFELY (considering ETC control)

        
   //     analogWrite(6, 255);
        lButtonReleaseTimeStamp = -1;
        bLaunchActive = 0;
      //  bBothButtonsPressed = 0;

        canMSG1.data[0] = 0xAA;
        canMSG1.data[1] = 0xAA;
        mcp2515.sendMessage(&canMSG1);
 //       mcp2515.sendMessage(&canMSG1);
 //       mcp2515.sendMessage(&canMSG1);
        
        SetLEDColor('r');
/*        analogWrite(5, 0);
        analogWrite(6, 255);
        analogWrite(9, 255); */
        delay(1000);
        analogWrite(5, 255);
        }
        
      }

   if ( IsBothButtonsPressed() ) //If dt < 500ms and both buttons are pressed
   {
     lButtonReleaseTimeStamp = -1;
   }
      
  }

  
  if((!IsBothButtonsPressed() && bLaunchActive) || (!IsBothButtonsPressed() && !bLaunchActive))
  {
    SetLEDColor(0);
/*    analogWrite(6, 255);
    analogWrite(5, 255);
    analogWrite(9, 255); */
    if ( millis() - timeStamp > 1000)
    {
      canMSG1.data[0] = 0x55;
      mcp2515.sendMessage(&canMSG1);
      timeStamp = millis();
    }
  }

 //Flappies-functionality//////////////////////////////////////////////////////////////////////////////////////////////////////////


 // ////////////// 

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



//CANbus////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void readCANbus()
{

    for (int i = 0; i < 8; i++)
    {
      IncMessageLocal[i] = 0;
    }
    IncMessageID = 0x6FF; 
    mcp2515.sendMessage(&debuggingMSG);    
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

void SendRotarySwitchData()
{

  if (millis() - rotaryTimeStamp > 100)
  {
  //Add send only when a state is changed
  outgoingMSG.can_id = 0xEE;
  outgoingMSG.can_dlc = 4;
  outgoingMSG.data[0] = return8bitRawPotmeterValue();
  outgoingMSG.data[1] = ActualPotmeterPosition();
  mcp2515.sendMessage(&outgoingMSG);
  
  rotaryTimeStamp = millis();
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
  for (int i = 0; i < 2; i++) //4 ink. flappies, flappies are currently not 
  {
  pinMode(iButtonPins[i], INPUT);
  }
}



//Usefull lines for debugging
//
//Serial.print(bButtonValues[0]); Serial.print(" "); Serial.print(bButtonValues[1]); Serial.print("\n"); Prints the left and right current button state
//Serial.print("1 \n"); Prints a number and newline, usefull for tracking the flow
