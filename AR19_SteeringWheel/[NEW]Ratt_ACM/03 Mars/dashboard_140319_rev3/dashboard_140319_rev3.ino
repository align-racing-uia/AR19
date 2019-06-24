#include <can.h>
#include <mcp2515.h>

////////////////////////////////////////
//For use in AR19 Dashboard ACM ////////
//Last Updated 140319 1928  ////////////
//By Marcus N. Løvdal //////////////////
////////////////////////////////////////
//Note: Buttons are used with //////////
//Pull-down-resistors //////////////////
////////////////////////////////////////



//Tested with two buttons successfully on an Uno, potmeter functionality successfully tested including hysteresis

//To do:
//Implement CANbus-functionality
//Implement Launch-Active Check
//Upload and test
//Gjør object-orientert (IKKE en prioritet............)

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
// WERY, WERY, WERY, WERY IMPORTANT //
// All variables connected to canbus (mcp2515.h) are only temporary MUST be discussed in plenium and concluded BEFORE actual implementation //
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //

//??? for testing ONLY: usage of serial functions (Serial.begin/print) and delay

  //CANbus 
  struct can_frame canMSG1, canMSG2, incommingMSG; 
  MCP2515 mcp2515(7); //Selcts pin for CANbus. IMPORTANT: must be 7 for ACM rev c

  //Pin number setup
  const int iButtonPins[4] = {2, 4, 7, 8}; //?Add ocrrect button pin numbers - In order: Left button, Right Button, Left Flappy, Right Flappy
  const int iRotarySwitchPin = A0; //?Add correct pin number

  //Initialize variables
  bool bButtonValues[4] = {0,0,0,0}; //lb, rb, lf, rf (In order: Left button, Right button, Left flappy, Right flappy)
  uint32_t uiRotarySwitchPositionRaw = 0;

  //Misc. Constants
  const long lReleaseWithin = 200; //(ms). Must be <= 500. ?Add better time

  //Misc. Variables
  bool bBothButtonsPressed = 0;
  bool bLaunchActive = 0; //?Current implementation will not reset this value
  long lButtonReleaseTimeStamp = -1; //? -1 means the TimeStamp is ready to recieve a new value. The timestamp will not be updated as defined in the function MakeTimeStamp();
  uint32_t defaultMessageLenght = 4; //? default CANbus message lenght TBD

  //Variables relevant for the rotary switch
  const uint32_t maxSupplyVoltageOffset = 10; //% of supply
  uint32_t operatingRange = 100 - maxSupplyVoltageOffset;
  uint32_t maxRawSignalValue = 1023 * operatingRange / 100; //default for arduino (?), 1024 alternatives (including 0)
  const uint32_t signalHysteresisValue = 20; //how much larger or smaller the value must be than the limit before triggering a value change. A mechanical solution would render this fairly unneccesary.
  const uint32_t numberOfIntervals = 8; //?example
  uint32_t switchBoundries[numberOfIntervals+1];
  uint32_t currentPotmeterPosition = 0; //Based on the intervals defined above
  uint32_t previousPotmeterPosition = 0; //The value sent over CANbus

  
 uint32_t MinimumBoundryValue()
 {
  return (switchBoundries[currentPotmeterPosition-1] + signalHysteresisValue);
 }

 uint32_t MaximumBoundryValue()
 {
   return (switchBoundries[currentPotmeterPosition] - signalHysteresisValue);
 }

 bool positionWithinHysteresis(uint32_t inputData)
 {
  if ((inputData > MinimumBoundryValue()) && inputData <  MaximumBoundryValue())
  { return 0; }
  else
  { return 1; }
}

 void SendRotarySwitchData()
 {
  if ( !positionWithinHysteresis(uiRotarySwitchPositionRaw)  )
  {
    //send current position
    Serial.print(currentPotmeterPosition); Serial.print(" "); Serial.print(uiRotarySwitchPositionRaw);
    previousPotmeterPosition = currentPotmeterPosition;
  }
  else
  {
    //send previous potmeter position
    Serial.print(previousPotmeterPosition); Serial.print(" "); Serial.print(uiRotarySwitchPositionRaw);
  }
 // Serial.print(uiRotarySwitchPositionRaw); Serial.print(" "); Serial.print(previousPotmeterPosition); Serial.print(" "); Serial.print(currentPotmeterPosition);
}



void setup() {
  //Initialize pins ///
  for (int i = 0; i < 4; i++)
  {
  pinMode(iButtonPins[i], INPUT);
  }
  pinMode(iRotarySwitchPin, INPUT);

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

  for (uint32_t i = 0; i < numberOfIntervals+1; i++) //Creates the interval boundries as an array for later reference
  {
    switchBoundries[i] = ( maxRawSignalValue / numberOfIntervals ) * i;
  }

  //Other ///
  
  Serial.begin(9600);

  for (uint32_t i = 0; i < numberOfIntervals+1; i++) //Creates the interval boundries as an array for later reference
  {
    switchBoundries[i] = ( maxRawSignalValue / numberOfIntervals ) * i;
  }

}

// Button/sensor inputs ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CheckButtons() 
  {
    for (int i = 0; i < 2; i++)
    {
      bButtonValues[i] = digitalRead(iButtonPins[i]);
    }
    
    uiRotarySwitchPositionRaw = analogRead(iRotarySwitchPin);
    currentPotmeterPosition = CalculatePotmeterPosition(uiRotarySwitchPositionRaw);
  }

uint32_t CalculatePotmeterPosition(uint32_t dataRaw) //Calculates current position given the raw data (or raw value minus the offset). First, it checks if the potmeter is in the first bracket. If it is, the function returns 1 and the function is complete. If not, won't return anything and check if the potmeter is in bracket 1 OR 2. If this turns out true, we know the sensor is in bracket 2, or else it would have returned a value last run; thus it returns 2 and the function is complete.
{
  for (int i = 1; i < numberOfIntervals+1; i++)
  {
    if (dataRaw < switchBoundries[i])
    {
      return i;
    }
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


void Reset() //??? For use under testing
{
  bBothButtonsPressed = 0;
  bLaunchActive = 0;
  lButtonReleaseTimeStamp = -1;
  for (int i = 0; i < 4; i++)
  {
    bButtonValues[i] = 0;
  }
  uiRotarySwitchPositionRaw = 0;
}

//Main////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
void loop() {
  // put your main code here, to run repeatedly:
//  Serial.print("1 \n");
  CheckButtons();
  SendRotarySwitchData();
 // Serial.print(uiRotarySwitchPositionRaw); Serial.print(" | "); Serial.print(MinimumBoundryValue()); Serial.print(" | "); Serial.print(MaximumBoundryValue()); Serial.print(" | "); Serial.print(positionWithinHysteresis(uiRotarySwitchPositionRaw));


  
 // Serial.print(uiRotarySwitchPositionRaw);
//  Serial.print(bButtonValues[0]); Serial.print(" "); Serial.print(bButtonValues[1]); Serial.print(" Button Values \n");


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
        
        Serial.print("2 \n");
  }
  else if (bBothButtonsPressed && bLaunchActive)  //if both buttons are confirmed held and Launch is currently active,
  {
    Serial.print("3 \n");
    if ( IsOneButtonNotPressed() ) //if one of the buttons are released,
      {
      MakeTimeStamp();
      Serial.print("4 \n");
      Serial.print( ButtonsReleasedWithinTime() ); Serial.print(" "); Serial.print( IsNoButtonsPressed() ); Serial.print(" dt < 500 and IsNoButtonsPressed \n");
      if ( ButtonsReleasedWithinTime() && IsNoButtonsPressed() ) //If dt < 500ms AND both buttons are released
        {
        //Confirmed Take-off
        //Send CANbus-message to LB to confirm takeoff
        lButtonReleaseTimeStamp = -1;
        bLaunchActive = 0;
        Serial.print("Confirmed Take-off");
        Reset();
        delay(3000); //???
        }
      
      else if (!ButtonsReleasedWithinTime()) //If dt > 500ms
        {
        //Take-off Failure
        //?Send and confirm CANbus-message to Launch Board that the vehicle should revert to pre-launch SAFELY (considering ETC control)
        Serial.print("TAKEOFF FAILURE");
        Reset();
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
  delay(10);
  
}


//Usefull lines for debugging
//
//Serial.print(bButtonValues[0]); Serial.print(" "); Serial.print(bButtonValues[1]); Serial.print("\n"); Prints the left and right current button state
//Serial.print("1 \n"); Prints a number and newline, usefull for tracking the flow
