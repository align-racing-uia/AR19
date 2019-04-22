#ifndef calculator
#define calculator

#include "Arduino.h"
#include "initialConditions.h"
#include "errorHandler.h"
#include "dataInput.h"




class Calculator
{

  private:
    ExternalSource** _ES;
    InitialConditions* _IC;
    ErrorHandler* _EH;
    uint16_t _componentID;
    
  public:
  Calculator(ExternalSource** ES, InitialConditions* IC, ErrorHandler* EH, uint16_t componentID);

  double slipInputCalculator();
  double slipSetpointCalculator();

  uint8_t mappingRPM(double regulatorOutput);
  uint8_t mappingCutter(double regulatorOutput);

};






#endif 