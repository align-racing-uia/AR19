

/*
===============================================2019==================================================
| This program is developed as a part of the bachelor projcet "Launch Control and Traction Control" |
| at the university of Agder. Its purposes is to serve as the main component of the systen.         |
| Developed for Align Racing UiA, by Henrik Brådland. Spring 2019.                                  |
=====================================================================================================
*/


#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>
#include <PID_v1.h>


#include "dataInput.h"
#include "dataOutput.h"
#include "errorHandler.h"
#include "initialConditions.h" 
#include "systemState.h"
#include "regulator.h"
#include "calculator.h"



 

//Tells the system what state it's in, ref: "Launch Control State Chart.docx"
uint8_t currentSystemState;
uint16_t componentID;
uint8_t CurrentGear;

unsigned long timeStampSate15, timeStampSate30, timeStampSate40, timeStampSate90, timeStampSate91, timeStampError, times, timeStampMessagesOK;

double setpointRPM, inputRPM, outputRPM, setpointSlip, inputSlip, outputSlip;

MCP2515 mcp2515(7);

ErrorHandler EH(2);
InitialConditions IC;
access userAccess(&EH, 9);



PID RegulatorPIDRPM(&inputRPM, &outputRPM, &setpointRPM, IC._KpPIDRPM, IC._KiPIDRPM, IC._KdPIDRPM, DIRECT), 
    RegulatorPIDSlip(&inputSlip, &outputSlip, &setpointSlip, IC._KpPIDSlip[0], IC._KiPIDSlip[0], IC._KdPIDSlip[0], REVERSE);

    

//External sources represented as objects 
SensorHall LeftFrontHall(&IC, &EH, 0, 10, 0x220),
           RightFrontHall(&IC, &EH, 0, 11, 0x210),
           DifferentialHall(&IC, &EH, 0, 12, 0x2D0),
           SprocketHall(&IC, &EH, 0, 13, 0x2E0),
           EngineSpeedHall(&IC, &EH, 0, 14, 0x2F0);

SensorPotentiometer StearingPot(&IC, &EH, 0, 20, 0x235),
                    GasPedal(&IC, &EH, 0, 21, 0x010)
                    GearPosition(&IC, &EH, 3, 22, 0x020);

SensorButton StearingBtn(&IC, &EH, 0, 30, 0x250),
             ClutchBtn(&IC, &EH, 0, 31, 0x2C8), 
             ETCBtn(&IC, &EH, 0, 32, 0x2B0),
             ACMping(&IC, &EH, 0, 33, 0x019);    

ICuppdater userInterface(&IC, &EH, &userAccess, 50, 0x448);

ExternalSource* externalSources[] = {&LeftFrontHall, &RightFrontHall, &DifferentialHall, &SprocketHall, &StearingPot, 
                                     &StearingBtn, &ClutchBtn, &GasPedal, &EngineSpeedHall, &ETCBtn, &GearPosition};

CanReader DataReader(&IC, &EH, 5, &mcp2515);
CanSender DataSender(&IC, &EH, 3, &mcp2515);
IgnitionCutter IgnitionSlayer(&IC, &EH, 8);

stateGuardian StateGuardian(externalSources, &currentSystemState, &IC, &EH, 4);
Calculator calculon(externalSources, &IC, &EH, 7);



void setup() {

  timeStampSate15 = 0;
  timeStampSate30 = 0;
  timeStampSate40 = 0;
  timeStampSate90 = 0;
  timeStampSate91 = 0;
  timeStampError = 0;
  times = 0;
  timeStampMessagesOK = 0;

  initializeController()

  CurrentGear = 0;

  componentID = 01;
  SPI.begin();
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS);
  mcp2515.setNormalMode();
 
}

void loop() {

  //Setting default values
  dataDistributer(externalSources, 11, DataReader.readMessages(), 6, &EH);



  if (ACMping.getDataU8 == IC._canMessageOk){

    DataSender.newMessage(IC._canIdOk, 1, IC._canMessageOk);
    ACMping.eraseData();

  }


 //   ======DEBUGGING========
  if(millis() - times > 100){

      DataSender.newMessage(IC._canIdSystemState, 1, currentSystemState);
      DataSender.newMessage(0x060, 1, StearingPot.getDataU8());
      DataSender.newMessage(0x061, 2, EH.getNumerOfErrors());
      
      DataSender.newMessage(0x062, 4, LeftFrontHall.getDataU32());
      DataSender.newMessage(0x063, 4, DifferentialHall.getDataU32());
      DataSender.newMessage(0x064, 4, StearingPot.getDataU32());      

      DataSender.newMessage(0x065, 1, calculon.mappingCutter(outputSlip));
      DataSender.newMessage(0x066, 4, calculon.slipInputCalculator());
      DataSender.newMessage(0x067, 1, StearingBtn.getDataU8());

      times = millis(); 
    }
  
  

  //Updating the system state, and if the system enters a new state, it sends a message
  if (StateGuardian.updateSystemState()){
      DataSender.newMessage(IC._canIdSystemState, 1, currentSystemState);
  }

  switch (currentSystemState)
  {
    case 1:  //Initialization

      // Do nothing, wait for driver to turn on the system.
    
    break;

    case 10:  //Pre-launch

      // Do nothing, wait for launch buttons to be presed.

    break;

    case 15:
      if (millis() - timeStampSate15 > IC._timeDelayMessagesState15Millis){

          if (ETCBtn.getDataU8() != IC._valueTrueBtn){
            DataSender.newMessage(IC._canIdCommunicationETC, 1, IC._canMessageRequestControlETC);
          }

          if (ClutchBtn.getDataU8() != IC._valueTrueBtn){
            DataSender.newMessage(IC._canIdCommunicationCluch, 1, IC._canMessageRequestControl);
          }

        timeStampSate15 = millis();
      }



    break;

    case 20:  //Launch Ready

    break;

    case 30:  //Launch Active - RPM regulering

      inputRPM = (double) EngineSpeedHall.getDataU32();
      if(RegulatorPIDRPM.Compute()){
          DataSender.newMessage(IC._canIdCommunicationETC, 2, IC._canMessageRelinquishControlETC + calculon.mappingRPM(outputRPM));
      }
      
    break;
    
    case 40:  //Take off - Slip regulating


      if (millis() - timeStampSate40 > IC._timeDelayMessagesState40Millis){

        if(ClutchBtn.getDataU8() == IC._valueTrueBtn){
          DataSender.newMessage(IC._canIdCommunicationCluch, 1, IC._canMessageRelinquishControl);
         
        }
        if (ETCBtn.getDataU8() == IC._valueTrueBtn){
              DataSender.newMessage(IC._canIdCommunicationETC, 1, IC._canMessageRelinquishControlETC);
          }

        inputSlip = calculon.slipInputCalculator();
        RegulatorPIDSlip.Compute();

        IgnitionSlayer.uppdateFrequency(calculon.mappingCutter(outputSlip));
        
        timeStampSate40 = millis();
      }

      IgnitionSlayer.slaughter();

    break;

    case 90:  //Safemode step 1/2
      if (millis() - timeStampSate90 > IC._timeDelayMessagesState90Millis){
          DataSender.newMessage(IC._canIdCommunicationETC, 1, 0);
          DataSender.newMessage(IC._canIdCommunicationCluch, 1, IC._canMessageAborte);
          DataSender.newMessage(IC._canIdSystemState, 1, currentSystemState);
          timeStampSate90 = millis();
          
          if(StearingBtn.getDataU8() == IC._valueFailBtn){
            EH.newError(201, componentID);
          }

      }

    break;

    case 91:  //Safemode step 2/2
      if(millis() - timeStampSate91 > IC._timeDelayMessagesState91Millis){
          DataSender.newMessage(IC._canIdCommunicationETC, 1, IC._canMessageRelinquishControl);
          DataSender.newMessage(IC._canIdSystemState, 1, currentSystemState);          
          timeStampSate91 = millis();    
      }

    break;

    default:

      EH.newError(003, componentID);

    break;
  }

  if (EH.getNumerOfErrors() > 0 && millis() - timeStampError > IC._timeDelayMessagesStateErrorMillis){

    DataSender.newMessage(IC._canIdErrorMessages, 4, EH.getErrorMessages());

    timeStampError = millis();
  }

}



void initializeController(){

  RegulatorPIDRPM.SetOutputLimits(IC._ControllerRPMOutputMin, IC._ControllerRPMOutputMax);
  RegulatorPIDSlip.SetOutputLimits(IC._ControllerSlipOutputMin, IC._ControllerSlipOutputMax);

  RegulatorPIDRPM.SetMode(AUTOMATIC);
  RegulatorPIDSlip.SetMode(AUTOMATIC);
  
  setpointRPM = IC._setPointRPM;

}

void uppdateRegulatorParameters(){

  if (CurrentGear != GasPedal.getDataU8() - 1){
      CurrentGear = GasPedal.getDataU8() - 1;
      RegulatorPIDSlip().SetTunings(IC._KpPIDSlip[CurrentGear], IC._KiPIDSlip[CurrentGear], IC._KpPIDSlip[CurrentGear]);
  }

}