#include <can.h>
#include <mcp2515.h>


// ØVerste ACM

unsigned volatile static long numberOfPulses = 0;
volatile static long previousTimeMicro = 0;
volatile static double timeBetweenPulses = 9999999999.0;


const uint16_t canID = 0x2F1; // canID: 0x2F1 -> Sprocket, 0x2F0 -> Diff, 0x220 -> FL, 0x210 -> FR
double hallPPR = 39.0;


double rpm = 0.0;
long numberOfPulsesLastTime = 0;
long exactTimeInterval;
long pulsesInInterval;
double rpmCountingMethod = 0.0;


unsigned long ThNaa = 0;
unsigned long ThForrige = 0;

const int interruptPin = 3;

unsigned long previousTimeAvansertMetode = 0;
unsigned long timeIntervallAvansertMetode = 250000; // microsecond

unsigned long numberOfPulsesArray[10];
unsigned long timeArray[10];
unsigned long ThForrigeArray[10];


// Oil measurements ////////////////////////


  struct can_frame oilMSG;
  struct can_frame oilMSGdebugPress1, oilMSGdebugTemp1, oilMSGdebugPress2, oilMSGdebugTemp2;

  union myUnion
  {
    uint16_t u16;
    uint8_t u8[2];
  };

  union floatToByte
  {
    float f;
    uint8_t u8[4];
  };
  
  
  union myUnion uniTemp, uniPress;
  union floatToByte uniFloatTemp, uniFLoatPress;

  const uint8_t oilTempPin = A4;
  const uint8_t oilPressurePin = A5;
  
  uint16_t oilTempValueRaw = 0;
  uint16_t oilPressureValueRaw = 0;
  
  uint8_t oilTempValue = 0;
  uint8_t oilPressureValue = 0;
  
  long oilSensorsTimeStamp = 0;

  uint16_t oilMsgDelay = 100;

  float curveFittingOilPressure[3] = {0.0000786, 0.04215, -0.421}; //[]x^2 + []x + []
  float oilPressureResistorValue, oilTempResistorValue;

  float curveFittingOilTemperature[3] = {0,0,0}; //TBD
  float fOilTemperatureConvertionFactor = 255/772;

  float fOilTempValue = 0;
  float fOilPressureValue = 0;

////////////////////////////////////////////////////////////

struct can_frame canMsg4;

MCP2515 mcp2515(7);

union Data
{
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


  // Oil Setup /////////////////////////////
  

  oilMSGdebugTemp1.can_id = 0x777;
  oilMSGdebugTemp1.can_dlc = 6;
  
  oilMSGdebugTemp2.can_id = 0x778;
  oilMSGdebugTemp2.can_dlc = 8;
  

  oilMSGdebugPress1.can_id = 0x779;
  oilMSGdebugPress1.can_dlc = 6;

  oilMSGdebugPress2.can_id = 0x77A;
  oilMSGdebugPress2.can_dlc = 8;



  oilMSG.can_id = 0x4A0;
  oilMSG.can_dlc = 2;
  
  for (uint8_t i = 0; i < oilMSG.can_dlc; i++)
    {
      oilMSG.data[i] = 0;
    }

  for (uint8_t i = 0; i < oilMSGdebugPress1.can_dlc; i++)
    {
      oilMSGdebugPress1.data[i] = 0;
    }
  
  for (uint8_t i = 0; i < oilMSGdebugTemp1.can_dlc; i++)
    {
      oilMSGdebugTemp1.data[i] = 0;
    }
  



  pinMode(oilTempPin, INPUT);
  pinMode(oilPressurePin, INPUT);


  /////////////////////////////////////



}

void loop()
{
  rpmAvansert();
  measureOil();

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

void measureOil() {

  oilTempValueRaw = analogRead(oilTempPin);
  oilPressureValueRaw = analogRead(oilPressurePin);

  uniTemp.u16 = oilTempValueRaw;
  uniPress.u16 = oilPressureValueRaw;

  for (uint8_t i = 0; i < 2; i++)
  {
    oilMSGdebugTemp1.data[i] = uniTemp.u8[i];
    oilMSGdebugPress1.data[i] = uniPress.u8[i];
  }
  

  oilTempResistorValue = 2.89 * (float) oilTempValueRaw - 33.72;
  oilPressureResistorValue = 7.91f * (float) oilPressureValueRaw - 69.1f;

  uniFloatTemp.f = oilTempResistorValue;
  uniFLoatPress.f = oilPressureResistorValue;

  for (uint8_t i = 0; i < 4; i++){
    oilMSGdebugTemp1.data[2+i] = uniFloatTemp.u8[i];
    oilMSGdebugPress1.data[2+i] = uniFLoatPress.u8[i];

    oilMSGdebugTemp2.data[i] = uniFloatTemp.u8[i];
    oilMSGdebugPress2.data[i] = uniFLoatPress.u8[i];
  }


  fOilTempValue = 217.22 - 26.8*log(oilTempResistorValue);
  oilTempValue = (uint8_t) fOilTempValue;
  uniFloatTemp.f = fOilTempValue;

  fOilPressureValue =  (curveFittingOilPressure[0]*oilPressureResistorValue*oilPressureResistorValue) + (curveFittingOilPressure[1]*oilPressureResistorValue) + curveFittingOilPressure[2];
  oilPressureValue = (uint8_t) (fOilPressureValue*10);
  uniFLoatPress.f = fOilPressureValue;

  for (uint8_t i = 0; i < 4; i++)
  {
    oilMSGdebugTemp2.data[4+i] = uniFloatTemp.u8[i];
    oilMSGdebugPress2.data[4+i] = uniFLoatPress.u8[i];
  }

  oilMSG.data[0] = oilTempValue;
  oilMSG.data[1] = oilPressureValue;


  if (millis() - oilSensorsTimeStamp > oilMsgDelay)
  {
      mcp2515.sendMessage(&oilMSG);
      mcp2515.sendMessage(&oilMSGdebugPress1);
      mcp2515.sendMessage(&oilMSGdebugPress2);
      mcp2515.sendMessage(&oilMSGdebugTemp1);
      mcp2515.sendMessage(&oilMSGdebugTemp2);
      oilSensorsTimeStamp = millis();
  }

 
}

void rpmAvansert()
{

  if(micros() - timeArray[0] > timeIntervallAvansertMetode/10)
  {  
    pulsesInInterval = numberOfPulses - numberOfPulsesArray[9];
    numberOfPulsesLastTime = numberOfPulses;
    ThNaa = (float)(micros() - previousTimeMicro);
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
        rpm =  1000000.0*60.0*(float)pulsesInInterval/(hallPPR*((float)(exactTimeInterval+ThForrige-ThNaa))); 
        rpm *= 1000; //setting the RPM to a higher value before converting to int
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


    if (rpm < 3000)
    {
    mcp2515.sendMessage(&canMsg4);
    }
  }
}
