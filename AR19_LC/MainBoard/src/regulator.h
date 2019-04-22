#ifndef regulator
#define regulator
#include "Arduino.h"
#include <PID_v1.h>



   double _Kp, _Ki, _Kd; 
    double _setpoint, _input, _output;

    PID _myPID(&_input, &_output, &_setpoint,_Kp, _Ki, _Kd, DIRECT);

    


class RegulatorPID
{

  private:
 

//    PID myPID(double* &_input, double* &_output, double* &_setpoint, double _Kp, double _Ki, double _Kd, int DIRECT);

  public:
  RegulatorPID(double Kp, double Ki, double Kd);


  
  void PIDRegulate(double input, double setpoint);
  double getOutput();
  
  
};


#endif 