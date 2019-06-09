////////////////////////////////////////
//For use in AR19 Dashboard ACM ////////
//Last Updated 120319 1852  ////////////
//By Marcus N. Løvdal //////////////////
////////////////////////////////////////
//Note: Buttons are used with //////////
//Pull-down-resistors //////////////////
////////////////////////////////////////



//Tested with two buttons successfully on an Uno

//To do:
//Implement CANbus-functionality
//Implement Launch-Active Check
//Implement Rotary Switch Functionality
//Upload and test

//Hvis sjåføren dytter inn igjen knappen innen 500ms skal det ikke være noe problem 

//??? for testing ONLY: usage of serial functions (Serial.begin/print) and delay

  //Pin number setup
  const int iButtonPins[4] = {2, 4, 7, 8}; //?Add ocrrect button pin numbers - In order: Left button, Right Button, Left Flappy, Right Flappy
  const int iRotarySwitchPin = A0; //?Add correct pin number

  //Initialize variables
  bool bButtonValues[4] = {0,0,0,0}; //lb, rb, lf, rf (In order: Left button, Right button, Left flappy, Right flappy)
  unsigned int uiRotarySwitchPositionRaw = 0;

  //Misc. Variables
  bool bBothButtonsPressed = 0;
  bool bLaunchActive = 0; //?Current implementation will not reset this value
  long lButtonReleaseTimeStamp = -1; //? -1 means the TimeStamp is ready to recieve a new value. The timestamp will not be updated as defined in the function MakeTimeStamp();

  //Misc. Constants
  const long lReleaseWithin = 1000; //(ms). Must be <= 500. ?Add better time
  const int iRotarySensorScalingFactor = 1; //Scaling factor for the rotary knob on Dashboard




void setup() {
  //Initialize pins
  for (int i = 0; i < 4; i++)
  {
  pinMode(iButtonPins[i], INPUT);
  }
  pinMode(iRotarySwitchPin, INPUT);
  //?Add CANbus-pins?


  Serial.begin(9600);
}

void CheckButtons() 
  {
    for (int i = 0; i < 2; i++)
    {
      bButtonValues[i] = digitalRead(iButtonPins[i]);
    }
    
    uiRotarySwitchPositionRaw = analogRead(iRotarySwitchPin);
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

void SendRotarySwitchData(int iDataInput)
{
  int iScaledData = iDataInput/iRotarySensorScalingFactor;
  //Send scaled data via CANbus to LB
 // Serial.print(iScaledData); 
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
  Serial.print("1 \n");
  CheckButtons();
  SendRotarySwitchData(uiRotarySwitchPositionRaw);
  Serial.print(bButtonValues[0]); Serial.print(" "); Serial.print(bButtonValues[1]); Serial.print("\n");


  //Button functionality///////////////////////////////////////////////////////////////////////////////////////////////////
  if ( IsBothButtonsPressed() && !bLaunchActive) //if both buttons are pressed AND Launch is currently not active/regulating, check if both buttons are held
  {
        //?Send CAN-message to LB that both buttons are pressed, ?confirm that this message has been recieved
        bBothButtonsPressed = 1;
        
        Serial.print("2 \n");
        bLaunchActive = 1; //?if and ONLY if confirmed with LB
  }
  else if (bBothButtonsPressed && bLaunchActive)  //if both buttons are confirmed held and Launch is currently active,
  {
    Serial.print("3 \n");
    if ( IsOneButtonNotPressed() ) //if one of the buttons are released,
      {
      MakeTimeStamp();
      Serial.print("4 \n");
      Serial.print( ButtonsReleasedWithinTime() ); Serial.print(" "); Serial.print( IsNoButtonsPressed() ); Serial.print("\n");
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
      
      else if ( ButtonsReleasedWithinTime() && IsBothButtonsPressed() ) //If dt < 500ms and both buttons are pressed
      {
        lButtonReleaseTimeStamp = -1;
        Serial.print("5 \n");
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
      
  }

  
  //Flappies-functionality//////////////////////////////////////////////////////////////////////////////////////////////////////////






  Serial.print("\n");
  delay(50);
  
}


//Usefull lines for debugging
//
//Serial.print(bButtonValues[0]); Serial.print(" "); Serial.print(bButtonValues[1]); Serial.print("\n"); Prints the left and right current button state
//Serial.print("1 \n"); Prints a number and newline, usefull for tracking the flow
