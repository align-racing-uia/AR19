//Breaklight Settings

//Global CAN Settings
const int breakPressureCANID = 0x410;

int breakPressure = 0 ;

const int pressureThresholdOn = 26; //Threshold i hex av bremsen på 0 - 255 (Brems på)
const int pressureThresholdOff = 25; //Threshold i hex av bremsen på 0 - 255 (Brems av)

const int breaklightout = 3;// Mosfet on digital pin 3

//Debug Settings
const int  rled = 5;
const int  bled = 6;
const int  gled = 9;  

//  TPS
const int pwmFrequency = 5000;
const int canIdTps = 0x460;
const int tpsOutPin = 10;
int tps2Value = 0;