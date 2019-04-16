
// Viktig at denne fila kjem først i kompileringa(etter GEAR_ACM)
    


namespace global{
    const uint8_t sant = 0xF0;
    const uint8_t tull = 0x0F;
}


namespace shutdown{  // CheckGear -  Declarations
    const uint8_t inputPin = 17;  // Make namespace for CheckGear
    uint8_t sensorInput;
    uint8_t voltage;
    uint8_t currentGear = 99;
    uint8_t newGear = 99;
} 




