#ifndef dataOutput
#define dataOutput

#include "Arduino.h"
#include <SPI.h>
#include <mcp2515.h>


#include "errorHandler.h"
#include "initialConditions.h"


class CanSender
{
    private:
        InitialConditions* _IC;
        ErrorHandler* _EH;
        uint16_t _componentID;

        MCP2515* _mcp2515;


    public:
        CanSender(InitialConditions*, ErrorHandler*, uint16_t, MCP2515*);
        void newMessage(uint32_t senderID, uint8_t dlc, uint32_t messageValue);
};

class IgnitionCutter
{
    private:
        ErrorHandler* _EH;
        InitialConditions* _IC;
        uint16_t _componentID;
        uint8_t _frequency;
        uint16_t _counter;
    public:
        IgnitionCutter(InitialConditions*, ErrorHandler*, uint16_t);
        void slaughter();
        void uppdateFrequency(uint8_t);
};


#endif 