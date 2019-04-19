#include "regulator.h"
#include "Arduino.h"



RegulatorPID::RegulatorPID(double Kp, double Ki, double Kd) 
{

_setpoint = 100;
_myPID.SetMode(AUTOMATIC);

}


void RegulatorPID::PIDRegulate(double input, double setpoint)
{

  
  //set setpoint maybe
  _input = input;
  _setpoint = setpoint;
 
  _myPID.Compute();
 

}

double RegulatorPID::getOutput(){
  return _output;
}