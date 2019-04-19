#include "calculator.h"
#include "Arduino.h"

Calculator::Calculator(ExternalSource** ES, InitialConditions* IC, ErrorHandler* EH, uint16_t componentID) : 
         _ES(ES), _IC(IC), _EH(EH), _componentID(componentID)
{

}

double Calculator::slipInputCalculator()
{
  double frontSpeed, rearSpeed;

  if (_ES[0]->getDataU32() > _ES[1]->getDataU32()){
    frontSpeed = (double) _ES[0]->getDataU32();
  }else{
    frontSpeed = (double) _ES[1]->getDataU32();
  }

  if (_ES[2]->getDataU32() > _ES[3]->getDataU32()){
    rearSpeed = (double) _ES[2]->getDataU32();
  }else{
    rearSpeed = (double) _ES[3]->getDataU32();
  }


  if (frontSpeed == 0){
    frontSpeed = 1;
  }

  return (rearSpeed - frontSpeed)/frontSpeed;

}



double Calculator::slipSetpointCalculator()
{

  return (double) 10; //for testing 
}




uint8_t Calculator::mappingCutter(double regulatorOutput){


  return 0xFF; //for testing
}




uint8_t Calculator::mappingRPM(double regulatorOutput){

  return map((int)regulatorOutput, 0, 0xFF, 0, 100);

}