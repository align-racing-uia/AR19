

  //Pin number setup
  const int iLeftButtonPin; //?Add correct pin number 
  const int iRightButtonPin; //?Add correct pin number
  const int iLeftFlappyPin; //?Add correct pin number
  const int iRightFlappyPin; //?Add correct pin number
  const int iRotarySwitchPin; //?Add correct pin number

  //Initialize variables
  bool bLeftButtonPressed = 0;
  bool bRightButtonPressed = 0;
  bool bLeftFlappyPulled = 0;
  bool bRightFlappyPulled = 0;
  unsigned int uiRotarySwitchPositionRaw = 0;

  bool bBothButtonsPressed = 0;
  bool bLaunchActive = 0; //?Set this value from messages over CANbus?
  const long lReleaseWithin = 500; //(ms). Must be <= 500. ?Add better time
  long lButtonReleaseTimeStamp;

  

void setup() {
  //Initialize pins
  pinMode(iLeftButtonPin, INPUT);
  pinMode(iRightButtonPin, INPUT);
  pinMode(iLeftFlappyPin, INPUT);
  pinMode(iRightFlappyPin, INPUT);
  pinMode(iRotarySwitchPin, INPUT);
  
}

void CheckButtons() 
  {
    bLeftButtonPressed = digitalRead(iLeftButtonPin);
    bRightButtonPressed = digitalRead(iRightButtonPin);
    bLeftFlappyPulled = digitalRead(iLeftFlappyPin);
    bRightFlappyPulled = digitalRead(iRightFlappyPin);
    uiRotarySwitchPositionRaw = analogRead(iRotarySwitchPin);
  }

bool IsBothButtonsPressed() //Returns true if both buttons are pressed (AND)
{
  return bLeftButtonPressed && bRightButtonPressed;
}

bool IsOneButtonNotPressed() //Returns true if one or more of the buttons are NOT pressed  
{
  return !bLeftButtonPressed || !bRightButtonPressed;
}

bool IsNoButtonsPressed() //Returns true if NO buttons are pressed (!OR)
{
  return !bLeftButtonPressed && !bRightButtonPressed;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
void loop() {
  // put your main code here, to run repeatedly:
  CheckButtons();

  if ( !bBothButtonsPressed && !bLaunchActive) //if both buttons are not held yet AND Launch is currently not active/regulating, check if both buttons are held
  {
    if ( IsBothButtonsPressed )
      {
        //?Send CAN-message to LB that both buttons are pressed, ?confirm that this message has been recieved
        bBothButtonsPressed = 1;
        bLaunchActive = 1;
      }
  }
  else if (bBothButtonsPressed && bLaunchActive)  //if both buttons are held and Launch is currently active,
  {
    if ( IsOneButtonNotPressed ) //if one of the buttons are released,
    {
      lButtonReleaseTimeStamp = millis();
      while (millis() - lButtonReleaseTimeStamp < lReleaseWithin) //?Change to a non-loop solution?
      {
        CheckButtons();
        if ( IsNoButtonsPressed )
        {
          //?send CANbus message that buttons where released successfully
          bBothButtonsPressed = 0;
          break; //?if I understand this function correctly
        } 
      }
      if (bBothButtonsPressed)
      {
        //?send CANbus message that buttons where NOT released successfully
        while(true)
        {
         //?We're fucked!
         //?send CANbus message stating we're fucked!  
        }
      }
    }
  }
}
