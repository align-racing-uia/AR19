////////////////////////////////////////
//For use in AR19 Dashboard ACM ////////
//Last Updated 120319 1852  ////////////
//By Marcus N. Løvdal //////////////////
////////////////////////////////////////


//To do:
//Implement CANbus-functionality
//Implement Launch-Active Check
//Implement Rotary Switch Functionality
//Upload and test

//??? for testing ONLY: usage of serial functions (Serial.begin/print) and delay

  //Pin number setup
  const int iLeftButtonPin = 2; //?Add correct pin number 
  const int iRightButtonPin = 4; //?Add correct pin number
  const int iLeftFlappyPin; //?Add correct pin number
  const int iRightFlappyPin; //?Add correct pin number
  const int iRotarySwitchPin; //?Add correct pin number

  //Initialize variables
  bool bLeftButtonPressed = 0;
  bool bRightButtonPressed = 0;
  bool bLeftFlappyPulled = 0;
  bool bRightFlappyPulled = 0;
  unsigned int uiRotarySwitchPositionRaw = 0;

  //Misc. Variables
  bool bBothButtonsPressed = 0;
  bool bLaunchActive = 0; //?Current implementation will not reset this value
  long lButtonReleaseTimeStamp = -1; //? -1 means the TimeStamp is ready to recieve a new value. The timestamp will not be updated as defined in the function MakeTimeStamp();

  //Misc. Constants
  const long lReleaseWithin = 500; //(ms). Must be <= 500. ?Add better time
  const int iRotarySensorScalingFactor; //Scaling factor for the rotary knob on Dashboard


void setup() {
  //Initialize pins
  pinMode(iLeftButtonPin, INPUT);
  pinMode(iRightButtonPin, INPUT);
  pinMode(iLeftFlappyPin, INPUT);
  pinMode(iRightFlappyPin, INPUT);
  pinMode(iRotarySwitchPin, INPUT);
  //?Add CANbus-pins?


  Serial.begin(9600);
}

void CheckButtons() 
  {
    bLeftButtonPressed = digitalRead(iLeftButtonPin);
    bRightButtonPressed = digitalRead(iRightButtonPin);
    bLeftFlappyPulled = digitalRead(iLeftFlappyPin);
    bRightFlappyPulled = digitalRead(iRightFlappyPin);
    uiRotarySwitchPositionRaw = analogRead(iRotarySwitchPin);
  }

//Conditions/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsBothButtonsPressed() //Returns true if both buttons are pressed (AND)
{
  return bLeftButtonPressed && bRightButtonPressed;
}

bool IsOneButtonNotPressed() //Returns true if one or more of the buttons are NOT pressed  (!AND)
{
  return !bLeftButtonPressed || !bRightButtonPressed;
}

bool IsNoButtonsPressed() //Returns true if NO buttons are pressed (!OR)
{
  return !bLeftButtonPressed && !bRightButtonPressed;
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
  return millis() - lButtonReleaseTimeStamp < lReleaseWithin;
}

void SendRotarySwitchData(int iDataInput)
{
  int iScaledData = iDataInput/iRotarySensorScalingFactor;
  //Send scaled data via CANbus to LB
}

//Main////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
void loop() {
  // put your main code here, to run repeatedly:
 // Serial.print("1 \n");
  Serial.print(bLeftButtonPressed); Serial.print(" "); Serial.print(bRightButtonPressed); Serial.print("\n"); //Prints current button pressed
  CheckButtons();
  Serial.print(bLeftButtonPressed); Serial.print(" "); Serial.print(bRightButtonPressed); Serial.print("\n");
  bool test = IsBothButtonsPressed();
  Serial.print(test); Serial.print(" \n");
  SendRotarySwitchData(uiRotarySwitchPositionRaw);


  //Button functionality///////////////////////////////////////////////////////////////////////////////////////////////////
  if ( IsBothButtonsPressed() && !bLaunchActive) //if both buttons are pressed AND Launch is currently not active/regulating, check if both buttons are held
  {
        //?Send CAN-message to LB that both buttons are pressed, ?confirm that this message has been recieved
        bBothButtonsPressed = 1;
        
        Serial.print("2 \n");
        //bLaunchActive = 1; //?if and ONLY if confirmed with LB
  }
  else if (bBothButtonsPressed && bLaunchActive)  //if both buttons are confirmed held and Launch is currently active,
  {
    if ( IsOneButtonNotPressed ) //if one of the buttons are released,
      {
      MakeTimeStamp();
    
      if ( ButtonsReleasedWithinTime && IsNoButtonsPressed ) //If dt < 500ms AND both buttons are released
        {
        //Confirmed Take-off
        //Send CANbus-message to LB to confirm takeoff
        //lButtonReleaseTimeStamp = -1;
        //bLaunchActive = 0;
        }
      else if (!ButtonsReleasedWithinTime) //If dt > 500ms
        {
        //Take-off Failure
        //?Send and confirm CANbus-message to Launch Board that the vehicle should revert to pre-launch SAFELY (considering ETC control)
        while(true); //???
        }
        
      }
    else //No buttons are released yet
      {
        //Do nothing
      }
      
  }

  
  //Flappies-functionality//////////////////////////////////////////////////////////////////////////////////////////////////////////






  Serial.print("\n");
  delay(450);
  
}
