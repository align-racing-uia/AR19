#include <SPI.h>
#include <mcp2515.h>

struct can_frame oilMSG;

MCP2515 mcp2515(7);

const uint8_t oilTempPin = A4;
const uint8_t oilPressurePin = A5;

uint16_t oilTempValueRaw = 0;
uint16_t oilPressureValueRaw = 0;

uint8_t oilTempValue = 0;
uint8_t oilPressureValue = 0;

long oilSensorsTimeStamp = 0;

void setup() {
  
  oilMSG.can_id = 0x4A0;
  oilMSG.can_dlc = 2;
  
  for (uint8_t i = 0; i < oilMSG.can_dlc; i++)
    {
      oilMSG.data[i] = 0;
    }
    
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();

  pinMode(oilTempPin, INPUT);
  pinMode(oilPressurePin, INPUT);
  
  
}

void loop() {

  oilTempValueRaw = analogRead(oilTempPin);
  oilPressureValueRaw = analogRead(oilPressurePin);

  oilTempValue = oilTempValueRaw/4; //TBD
  oilPressureValue = oilPressureValueRaw/4; //TBD

  oilMSG.data[0] = oilTempValue;
  oilMSG.data[1] = oilPressureValue;


  if (millis() - oilSensorsTimeStamp > 1000)
  {
      mcp2515.sendMessage(&oilMSG);
      oilSensorsTimeStamp = millis();
  }


}
