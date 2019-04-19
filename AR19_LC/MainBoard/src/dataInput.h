#ifndef dataInput
#define dataInput

#include "Arduino.h"
#include "initialConditions.h"
#include "errorHandler.h"

#include <SPI.h>
#include <mcp2515.h>


struct CanMessage
{
    uint32_t canID;
    uint32_t data;
};

struct data
{
    unsigned long  time;
    uint32_t value;
};




class ExternalSource
{
    public:
    InitialConditions* _IC;
    ErrorHandler* _EH;
    uint16_t _componentID;
    uint16_t _canID;
    uint8_t _offSet;
    uint8_t _dataSize;
  
    
    ExternalSource(InitialConditions*, ErrorHandler*, uint8_t, uint8_t, uint16_t, uint16_t);
    virtual ~ExternalSource() {}

    virtual void newData(uint32_t value) =0;
    virtual bool verificationData(uint32_t value) = 0;
    virtual uint16_t getData() = 0;  

    virtual uint8_t getDataU8() = 0;    
    virtual uint16_t getDataU16() = 0;  
    virtual uint32_t getDataU32() = 0; 
    virtual uint64_t getDataU64() = 0;
    virtual int16_t getData16() = 0;
    virtual int32_t getData32() = 0;  
    virtual float getDataF() = 0;  
    virtual double getDataD() = 0;
    virtual void eraseData() = 0;

    uint16_t getComponentID();
    uint16_t getCanID();
};
 

void dataDistributer(ExternalSource** pES, uint16_t n, CanMessage data, uint16_t componentID, ErrorHandler* EH);



class SensorButton : public ExternalSource
{
        
    private:
    uint8_t _SensorButtonPressed;

    public:
    SensorButton(InitialConditions* IC, ErrorHandler* EH, uint8_t offSet, uint16_t componentID, uint16_t canID);
    virtual ~SensorButton() {}

    virtual void newData(uint32_t value);
    virtual bool verificationData(uint32_t value);
    virtual uint16_t getData();  

    virtual uint8_t getDataU8();    
    virtual uint16_t getDataU16();  
    virtual uint32_t getDataU32(); 
    virtual uint64_t getDataU64();  
    virtual int16_t getData16();
    virtual int32_t getData32();  
    virtual float getDataF();  
    virtual double getDataD();  
    virtual void eraseData();


};



class SensorPotentiometer : public ExternalSource
{
    private:
    uint8_t _sensorPotentiometerData;

    public:
    SensorPotentiometer(InitialConditions* IC, ErrorHandler* EH, uint8_t offSet, uint16_t componentID, uint16_t canID);
    virtual ~SensorPotentiometer() {}

    virtual void newData(uint32_t value);
    virtual bool verificationData(uint32_t value);
    virtual uint16_t getData();  

    virtual uint8_t getDataU8();    
    virtual uint16_t getDataU16();  
    virtual uint32_t getDataU32(); 
    virtual uint64_t getDataU64();  //long long? trudde 64 bit var long
    virtual int16_t getData16();
    virtual int32_t getData32();  
    virtual float getDataF();  
    virtual double getDataD();  
    virtual void eraseData();

};



class SensorHall : public ExternalSource
{  
    private:
    data _sensorHallData[10];

    public:
    SensorHall(InitialConditions* IC, ErrorHandler* EH, uint8_t offSet, uint16_t componentID, uint16_t canID);
    virtual ~SensorHall() {}
    
    virtual void newData(uint32_t value);
    virtual bool verificationData(uint32_t value);
    virtual uint16_t getData();  

    virtual uint8_t getDataU8();    
    virtual uint16_t getDataU16();  
    virtual uint32_t getDataU32(); 
    virtual uint64_t getDataU64();  //long long? trudde 64 bit var long
    virtual int16_t getData16();
    virtual int32_t getData32();  
    virtual float getDataF();  
    virtual double getDataD();  
    virtual void eraseData();
    
};


class CanReader {

    private:
    InitialConditions* _IC;
    ErrorHandler* _EH;
    uint16_t _componentID;
    uint16_t _canIDs[20];
    struct can_frame _canMsg;
    MCP2515* _mcp2515;



    public:
    CanReader(InitialConditions* IC, ErrorHandler* EH, uint16_t componentID, MCP2515* mcp2515);

    CanMessage readMessages();

};

#endif 