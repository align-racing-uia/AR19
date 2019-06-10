#ifndef safety
#define safety

#include "Arduino.h"
#include "initialConditions.h"
#include "errorHandler.h"
#include "dataInput.h"


class Safety
{¨

  private:
    ExternalSource** _ES;
    InitialConditions* _IC;
    ErrorHandler* _EH;
    uint16_t _componentID;
    
  public:
  Safety(ExternalSource** ES, InitialConditions* IC, ErrorHandler* EH, uint16_t componentID);

  bool hallSensorsDown();


}


#endif 