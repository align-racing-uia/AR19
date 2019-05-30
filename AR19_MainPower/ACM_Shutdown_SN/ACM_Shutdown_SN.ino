#include <SPI.h>
#include <mcp2515.h>
#include "settings.h"

struct can_frame canMsg;
MCP2515 mcp2515(7);

int time_now = 0;


void setup() {
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();

  pinMode(activatePin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

}

void loop() {
  
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    if (canMsg.can_id == shutdownCanId)
    {
      time_now = millis();
      while(millis() < time_now + SwitchDuration){ //wait approx. [period] ms
        digitalWrite(activatePin, LOW);
        digitalWrite(ledPin, HIGH);
      }
      
      
    }
    else
    {
      digitalWrite(activatePin, HIGH);
      digitalWrite(ledPin, LOW);
    }
  }
}
