#include <can.h>
#include <mcp2515.h>

unsigned volatile static long numberOfPulses = 0;
volatile static long previousTimeMicro = 0;
volatile static double timeBetweenPulses=9999999999.0;

    long rpm = 0.0;
  //  long previousTimeMicroCountingMethod = 0;
    long numberOfPulsesLastTime = 0;
    long exactTimeInterval;
    long pulsesInInterval;
    double rpmCountingMethod = 0.0;
    double hallPPR = 24.0;

    uint8_t carSpeed =0;
 
    
    long ThNaa = 0;
    long ThForrige = 0;
    
    const int interruptPin = 3;

    long previousTimeAvansertMetode = 0;
    long timeIntervallAvansertMetode = 250000; // microsecond

    long numberOfPulsesArray[10];
    long timeArray[10];
    long ThForrigeArray[10];

struct can_frame canMsg1;
//struct can_frame canMsg2;
//struct can_frame canMsg3;
//struct can_frame canMsg4;
//struct can_frame canMsg5;
 struct can_frame canMsg6;
MCP2515 mcp2515(7);

union Data{
    long longNum;
    byte arrNum[4];
  };
  
union Data data;

long  timeForRPMPrint ;
long  timeForCarSpeed;

void setup() {
  //  Serial.begin(9600); 
 // Serial.begin(9600); 
 // pinMode(interruptPin, INPUT_PULLUP);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), interFunck, RISING);
   //previousTimeMicroCountingMethod = micros();
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);

  canMsg1.can_id  = 0x220;  // front left
  canMsg1.can_dlc = 4;
  canMsg1.data[0] = 0;
  canMsg1.data[1] = 0x87;
  canMsg1.data[2] = 0x32;
  canMsg1.data[3] = 0xFA;


  
   canMsg5.can_id  = 0x218;  // fron Left  KM/T 
  canMsg5.can_dlc = 1;
  canMsg5.data[0] = 0;
  

//  canMsg2.can_id  = 0x210;  //front right
//  canMsg2.can_dlc = 4;
//  canMsg2.data[0] = 255;
//  canMsg2.data[1] = 0x00;
//  canMsg2.data[2] = 0x00;
//  canMsg2.data[3] = 0x08;

//  canMsg3.can_id  = 0x2d0;   // rear diff
//  canMsg3.can_dlc = 4;
//  canMsg3.data[0] = 255;
//  canMsg3.data[1] = 0x00;
//  canMsg3.data[2] = 0x00;
//  canMsg3.data[3] = 0x08;
////
////
//  canMsg4.can_id  = 0x2d1; // sprocket
//  canMsg4.can_dlc = 4;
//  canMsg4.data[0] = 255;
//  canMsg4.data[1] = 0x00;
//  canMsg4.data[2] = 0x00;
//  canMsg4.data[3] = 0x08;
// 


//   canMsg6.can_id  = 0x01;  //shutdown signal
//  canMsg6.can_dlc = 1;
//  canMsg6.data[0] = 0x01;


  SPI.begin();
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();

   timeForRPMPrint = micros();
 
}

void loop() {
 rpmAvansert();
faultyCheck();


   if(200000<micros()-timeForCarSpeed){
        //carSpeed = (uint8_t)(rpm*2*3.141593*0.2032*60/(1000*1000));
      carSpeed = (uint8_t)((float)((float)rpm*2.0f*3.14f*0.2032f*60.0f/(1000.0f*1000.0f)));
      canMsg5.data[0] = carSpeed;    
      mcp2515.sendMessage(&canMsg5);  
  
       Serial.print(rpm);
      Serial.print("   " );
      Serial.println(carSpeed);
                   
      timeForCarSpeed = micros();
    }

//if(250000<micros()-timeForRPMPrint){
  //      Serial.println(rpm);
//    //Serial.print("   " );
//  //  Serial.println(rpmCountingMethod);
  //     timeForRPMPrint = micros();
  //}

    
  if(rpm == 0.0){
     analogWrite(5,0);
      analogWrite(6,0);
      analogWrite(9,0); 
  }

  if(rpm > 0.0 && 10.0 > rpm){
     analogWrite(5,0);
      analogWrite(6,255);
      analogWrite(9,255); 
  }

  if(rpm > 10.0 && 25.0 > rpm){
     analogWrite(5,255);
      analogWrite(6,0);
      analogWrite(9,255); 
  }

  if(rpm > 25.0){
     analogWrite(5,255);
      analogWrite(6,255);
      analogWrite(9,0); 
  }


}


void interFunck() {
  numberOfPulses++;
  // timeBetweenPulses = (double)micros()-(double)previousTimeMicro;
  previousTimeMicro = micros();
 //Serial.println(numberOfPulses);
}



void rpmAvansert(){

  if(micros()- timeArray[0] > timeIntervallAvansertMetode/10){
    pulsesInInterval = numberOfPulses - numberOfPulsesArray[9];
   numberOfPulsesLastTime = numberOfPulses;
    ThNaa = (double)(micros()- previousTimeMicro);
    exactTimeInterval = micros()- timeArray[9];
    previousTimeAvansertMetode = micros();

    //Serial.print(exactTimeInterval);
    //Serial.print("   ");
    //Serial.println(ThNaa);

    
    if(ThForrige !=0){

      if(pulsesInInterval == 0){
        //rpm = 0.0;
        
        if(micros()-previousTimeMicro> 500000){
                rpm = 0;
        }else{
           // rpm = 60.0*1000000.0/(hallPPR*((double)micros()-previousTimeMicro));
          }
      }else{
              
              rpm =  (long)(1000*1000000.0*60.0*pulsesInInterval/(hallPPR*((exactTimeInterval+ThForrige-ThNaa)))); 
              /* 
               Serial.print(" pulses  "); 
               Serial.print(pulsesInInterval);
              Serial.print(" interval   ");
              Serial.print((double)exactTimeInterval);
              Serial.print(" Thfor   ");
              Serial.print((double)ThForrige);   
              Serial.print(" Thnaa   ");
              Serial.print((double)ThNaa); 
              Serial.print(" rpm   ")|1;
              Serial.print(rpm);
              Serial.println("   ");
              */
              
          }
     // Serial.println(pulsesInInterval);
     }
  

    for(int i = 0; i < 9; i = i + 1){
   numberOfPulsesArray[9-i] = numberOfPulsesArray[8-i];
   timeArray[9-i] =  timeArray[8-i];
   ThForrigeArray[9-i] = ThForrigeArray[8-i];
    }
  
  ThForrigeArray[0] = ThNaa;
  numberOfPulsesArray[0] = numberOfPulses;
  timeArray[0] = micros();

    ThForrige =ThForrigeArray[9];

        data.longNum =rpm;

    for (int i = 0; i < 4; i++){                 // Receive the raw 'float' data.
         canMsg1.data[3-i] = data.arrNum[i];    //    "     "     "     "     "   
         //canMsg2.data[3-i] = data.arrNum[i]; 
         //canMsg3.data[3-i] = data.arrNum[i]; 
        // canMsg4.data[3-i] = data.arrNum[i];       
    }
  
    mcp2515.sendMessage(&canMsg1);
   // mcp2515.sendMessage(&canMsg2);
  ///   mcp2515.sendMessage(&canMsg3);
   //   mcp2515.sendMessage(&canMsg4);
    
    }
  }

  void faultyCheck(){
     if(3000<rpm){
      mcp2515.sendMessage(&canMsg6);
    }
  }

  
 
