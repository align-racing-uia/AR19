#include "Arduino.h"

struct can_frame msgIn;                             //  can_frame struct for reading incoming messages

const bool calibrateMode                    = false;    //  Set true for calibration

const uint8_t globalTrue                    = 0xF0;     //  Global true value for CAN use
const uint8_t globalFalse                   = 0x0F;     //  Global false value for CAN use

bool pwmSuccess1                            = false;    //  Bool set true if PWM setup is successful
bool pwmSuccess2                            = false;    //  Bool set true if PWM setup is successful
bool bootSuccess                            = false;    //  Bool set true if boot is successful

bool blipMode                               = false;
bool lcMode                                 = false;

namespace pwm
{
const uint16_t frequency                    = 20000;    //  PWM frequency
}

namespace etb
{
const uint8_t enable                        = 11;       //  H-Bridge Enable
const uint8_t disable                       = 12;       //  H-Bridge Disable
const uint8_t feedback                      = 19;       //  H-Bridge Feedback
const uint8_t directionPinFwd               = 10;       //  H-Bridge Out 1
const uint8_t directionPinRev               = 3;        //  H-bridge Out 2
bool safeState                              = false;
uint8_t throttlePosition;
}

namespace canbus 
{
const uint16_t acmPingId                    = 0x19;     //  CAN ID for incoming ping request
const uint16_t acmOkId                      = 0x16;     //  CAN ID for outgoing ping reply
const uint16_t pedalboxImplausibilityId     = 0x10;     //  CAN ID for outgoing implausibility message
const uint16_t appsId                       = 0x11;     //  CAN ID for incoming APPS data
const uint16_t requestBlipId                = 0x20;     //  CAN ID for incoming blip request
const uint16_t tpsId                        = 0x460;    //  CAN ID for outgoing TPS data
const uint16_t launchModeConfirmedId        = 0x2B0;    //  CAN ID for outgoing launch confirmation
const uint16_t blipConfirmedId              = 0x2C0;    //  CAN ID for outgoing blip confirmed
const uint16_t etbSafeStateId               = 0x470;    //  CAN ID for outgoing message if ETB is set in safe state
uint8_t throttleTarget, accPedalDirection;              //  Variables for throttle target and pedal direction
}

namespace pid
{
const unsigned long sampleTime_ms           = 1;        //  How often the PID should calculate in milliseconds
const double outputLimitMin                 = -250;     //  Minimun value of PID output
const double outputLimitMax                 = 250;      //  Maximum value of PID output
double kp                                   = 1.00;     //  Proportional parameter of PID
double ki                                   = 3.20;     //  Integral parameter of PID
double kd                                   = 12.50;     //  Derivative parameter of PID
uint8_t controllerDirection                 = DIRECT;   //  PID controller direction (DIRECT/INVERSE)
double input                                = 0;        //  Declaration of PID variables. 
double output                               = 0;
double setpoint                             = 0;
}

namespace tps 
{
const uint8_t sensor1Pin                    = 17;       //  TPS1 pin
const uint8_t sensor2Pin                    = 16;       //  TPS2 pin
const uint16_t value1Min                    = 103;      //  Physically min value of TPS1 103
const uint16_t value1Idle                   = 193;      //  Idle position of TPS1 193
const uint16_t value1Max                    = 903;      //  Physically max value of TPS1 903
const uint16_t value2Min                    = 919;      //  Physically min value of TPS2 919
const uint16_t value2Idle                   = 829;      //  Idle position of TPS2 829
const uint16_t value2Max                    = 119;      //  Physically max value of TPS2 119
uint8_t implausibility;
}

namespace time 
{
const unsigned long tpsInterval             = 100;
const unsigned long blipDurationInterval    = 100;
const unsigned long blipLastInterval        = 1000;
unsigned long tps, blip, blipLast;
}
