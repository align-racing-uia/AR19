#include <can.h>
#include <mcp2515.h>

unsigned volatile static long numberOfPulses = 0;
volatile static long previousTimeMicro = 0;
volatile static double timeBetweenPulses = 9999999999.0;


const uint16_t canID = 0x2F1; // canID: 0x2F1 -> Sprocket, 0x2F0 -> Diff, 0x220 -> FL, 0x210 -> FR
double hallPPR = 24.0;


double rpm = 0.0;
long numberOfPulsesLastTime = 0;
long exactTimeInterval;
long pulsesInInterval;
double rpmCountingMethod = 0.0;


long ThNaa = 0;
long ThForrige = 0;

const int interruptPin = 3;

long previousTimeAvansertMetode = 0;
long timeIntervallAvansertMetode = 250000; // microsecond

long numberOfPulsesArray[10];
long timeArray[10];
long ThForrigeArray[10];

struct can_frame canMsg4;
MCP2515 mcp2515(7);

union Data{
  uint32_t intNum;
  byte arrNum[4];
};
  
union Data data;

long  timeForRPMPrint ;

void setup()
{
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), interFunck, RISING);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);

  canMsg4.can_id  = canID;
  canMsg4.can_dlc = 4;
  canMsg4.data[0] = 255;
  canMsg4.data[1] = 0x00;
  canMsg4.data[2] = 0x00;
  canMsg4.data[3] = 0x08;

  SPI.begin();
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();

  timeForRPMPrint = micros();
}

void loop()
{
  rpmAvansert();

  if(250000<micros()-timeForRPMPrint)
  {
    timeForRPMPrint = micros();
  }
}

void interFunck() 
{
  numberOfPulses++;
  previousTimeMicro = micros();
}

void rpmAvansert()
{

  if(micros()- timeArray[0] > timeIntervallAvansertMetode/10)
  {  
    pulsesInInterval = numberOfPulses - numberOfPulsesArray[9];
    numberOfPulsesLastTime = numberOfPulses;
    ThNaa = (double)(micros() - previousTimeMicro);
    exactTimeInterval = micros() - timeArray[9];
    previousTimeAvansertMetode = micros();

    if(ThForrige !=0)
    {
      if(pulsesInInterval == 0  && micros() - previousTimeMicro > 500000)
      {
        rpm = 0;
      }
      else
      {
        rpm =  1000000.0*60.0*(double)pulsesInInterval/(hallPPR*((double)(exactTimeInterval+ThForrige-ThNaa))); 
      }
     }
  

  for(int i = 0; i < 9; i = i + 1)
  {
    numberOfPulsesArray[9-i] = numberOfPulsesArray[8-i];
    timeArray[9-i] =  timeArray[8-i];
    ThForrigeArray[9-i] = ThForrigeArray[8-i];
  }
  
  ThForrigeArray[0] = ThNaa;
  numberOfPulsesArray[0] = numberOfPulses;
  timeArray[0] = micros();

  ThForrige = ThForrigeArray[9];

  data.intNum = (uint32_t) rpm;

  for (int i = 0; i < 4; i++)
  {
    canMsg4.data[i] = data.arrNum[i];       
  }
  mcp2515.sendMessage(&canMsg4);
  }
}
