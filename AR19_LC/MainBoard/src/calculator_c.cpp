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

  return (rearSpeed - frontSpeed)/frontSpeed * 100; //Slip in %

}



double Calculator::slipSetpointCalculator()
{

  if (_ES[4]->getDataU8() == _IC->_errorPot){

      _EH->newError(202, _componentID);
      return 0;

  }else if (_ES[4]->getDataU8() == _IC->_minValuePot) {

      return 0;

  }
  
  return _IC->_setPointSlip[_ES[4]->getDataU8()-1];

}




uint8_t Calculator::mappingCutter(double regulatorOutput){


  return (uint8_t) regulatorOutput; //for testing
}




uint8_t Calculator::mappingRPM(double regulatorOutput){

  return map((int)regulatorOutput, 0, 0xFF, 0, 100);

}