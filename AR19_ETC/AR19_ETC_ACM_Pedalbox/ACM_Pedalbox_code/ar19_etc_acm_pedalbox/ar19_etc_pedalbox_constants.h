#include "Arduino.h"

struct can_frame msgIn;                                 //  can_frame struct for reading incoming messages

bool calibrateMode                    = false;    //  Set true for calibration

const uint8_t globalTrue                    = 0xF0;     //  Global true value for CAN use
const uint8_t globalFalse                   = 0x0F;     //  Global false value for CAN use

bool bootSuccess                            = false;    //  Initialisation of bootSucces bool

namespace canbus {
const uint16_t acmPingId                    = 0x19;     //  ACM request OK CAN ID
const uint16_t acmOkId                      = 0x12;     //  Pedalbox OK CAN ID
const uint16_t appsId                       = 0x11;     //  APPS data CAN ID
const uint16_t bpsId                        = 0x410;    //  BPS data CAN ID
const uint16_t pedalboxImplausibilityId     = 0x10;     //  Pedalbox implausibility CAN ID
const uint16_t calibrateModeToggleId        = 0x0F0;    //  Toggles calibrate mode CAN ID
const unsigned long impInterval_ms          = 10;       //  Interval to send implausibility
unsigned long impLast_ms                    = 0;        //  Timestamp for implausibility ov CAN
}

namespace apps {
const uint8_t sensor1Pin                    = 17;       //  Pin for APPS1 17
const uint8_t sensor2Pin                    = 16;       //  Pin for APPS2 16
const uint16_t value1Min                    = 205;      //  Min value for APPS1 212
const uint16_t value1Max                    = 412;      //  Max value for APPS1 605
const uint16_t value2Min                    = 205;      //  Min value for APPS2 189
const uint16_t value2Max                    = 412;      //  Max value for APPS2 401
const unsigned long impInterval_ms          = 100;      //  How long the implausibility has to last before trigger
unsigned long implausibleLast_ms            = 0;        //  Timestamp for implausibility 
uint8_t implausible, implausibilityOutOfRange1, implausibilityOutOfRange2, implausibilityDifference;
}

namespace bps {
const uint8_t sensor1Pin                    = 18;       //  Pin for BPS1 18
const uint8_t sensor2Pin                    = 19;       //  Pin for BPS2 19
const uint16_t value1Min                    = 80;       //  Min value for BPS1        
const uint16_t value1Max                    = 1023;     //  Max value for BPS1
const uint16_t value2Min                    = 80;       //  Min value for BPS2
const uint16_t value2Max                    = 1023;     //  Max value for BPS2
const unsigned long impInterval_ms          = 100;      //  How long the implausibility has to last before trigger
unsigned long implausibleLast_ms            = 0;        //  Timestamp for implausibility 
uint8_t implausible, implausibilityOutOfRange1, implausibilityOutOfRange2, implausibilityDifference;
}
