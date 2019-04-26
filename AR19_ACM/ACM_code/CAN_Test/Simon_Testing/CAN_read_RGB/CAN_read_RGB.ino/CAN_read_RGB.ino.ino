#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(7);

int R = 5;
int G = 6;
int B = 9;
int Delay = 10;
int CAN_ID=3;

void setup() {
  Serial.begin(9600);
  SPI.begin();


  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);


 
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() {
  
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {

    //Serial.print(canMsg.data[]);
    
    if(canMsg.data[0] == 0x00 ){
     digitalWrite(R, HIGH);   
     delay(Delay);               
     digitalWrite(R, LOW);  
     delay(Delay);
    }
    
    if(canMsg.data[0] == 0x01 ){
     digitalWrite(G, HIGH);   
     delay(Delay);               
     digitalWrite(G, LOW);  
     delay(Delay);  
    }
    if(canMsg.data[0] == 0x02 ){
     digitalWrite(B, HIGH);   
     delay(Delay);               
     digitalWrite(B, LOW);  
     delay(Delay);  
    }
    
    
    Serial.print(canMsg.can_dlc);
    Serial.print("-");
    Serial.print(canMsg.can_id, HEX);
    Serial.print("-");
    Serial.print(canMsg.data[1], HEX);
   
    
    
    /*
    for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
        
      Serial.print(canMsg.data[i],HEX);
      Serial.print("-");

    }
    */

    Serial.println();      
  }

}
