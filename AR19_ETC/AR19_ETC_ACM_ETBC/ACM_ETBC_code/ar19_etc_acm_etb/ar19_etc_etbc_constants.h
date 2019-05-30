#include "Arduino.h"

struct can_frame msgIn;                             //  can_frame struct for reading incoming messages

bool calibrateMode                          = false;    //  Set true for calibration
bool limpMode                               = false;
bool blipMode                               = false;
bool launchControlMode                      = false;

const uint8_t globalTrue                    = 0xF0;     //  Global true value for CAN use
const uint8_t globalFalse                   = 0x0F;     //  Global false value for CAN use

bool pwmSuccess1                            = false;    //  Bool set true if PWM setup is successful
bool pwmSuccess2                            = false;    //  Bool set true if PWM setup is successful
bool bootSuccess                            = false;    //  Bool set true if boot is successful

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
uint8_t throttlePosition;
//  ETB limp compensation
uint16_t value1, value2;
const uint16_t limp                         = 193;      //  Median of limp range 193
const uint16_t limp_pos                     = 225;      //  Positive top of limp range 225
const uint16_t limp_neg                     = 160;      //  Negative botto of limp range 160
const uint8_t limpCompensation              = 95;       //  Compensation for limp range due to opposing springs in ETB
double compensation;
}

namespace canbus 
{
const uint16_t acmPingId                    = 0x19;     //  CAN ID for incoming ping request
const uint16_t acmOkId                      = 0x16;     //  CAN ID for outgoing ping reply
const uint16_t pedalboxImplausibilityId     = 0x10;     //  CAN ID for outgoing implausibility message
const uint16_t appsId                       = 0x11;     //  CAN ID for incoming APPS data
const uint16_t requestBlipId                = 0x20;     //  CAN ID for incoming blip request
const uint16_t tpsId                        = 0x460;    //  CAN ID for outgoing TPS data
const uint16_t launchModeRequest            = 0x290;    //  CAN ID for incoming LC request and throttle target for launch
const uint16_t launchModeConfirmedId        = 0x2B0;    //  CAN ID for outgoing launch confirmation
const uint16_t blipConfirmedId              = 0x2C0;    //  CAN ID for outgoing blip confirmed
const uint16_t etbLimpModeId                = 0x470;    //  CAN ID for outgoing message if ETB is set in limp mode
const uint16_t calibrateModeToggleId        = 0x0F0;
uint8_t throttleTarget, throttleTargetLaunch, accPedalDirection;  //  Variables for throttle target and pedal direction
uint8_t launchModeConfirmedMsgCounter       = 0;                 
}

namespace pid
{
const unsigned long sampleTime_ms           = 1;        //  How often the PID should calculate in milliseconds
const double outputLimitMin                 = -250;     //  Minimun value of PID output
const double outputLimitMax                 = 250;      //  Maximum value of PID output
double kp                                   = 1.00;     //  Proportional parameter of PID 1.0
double ki                                   = 3.30;     //  Integral parameter of PID 3.30
double kd                                   = 22.50;    //  Derivative parameter of PID 22.50
uint8_t controllerDirection                 = DIRECT;   //  PID controller direction (DIRECT/INVERSE)
double input                                = 0;        //  Declaration of PID variables. 
double output                               = 0;
double setpoint                             = 0;
}

namespace tps 
{
const uint8_t sensor1Pin                    = 16;       //  TPS1 pin
const uint8_t sensor2Pin                    = 17;       //  TPS2 pin
const uint16_t value1Min                    = 103;      //  Physically min value of TPS1 103
const uint16_t value1Idle                   = 148;      //  Idle position of TPS1 1148
const uint16_t value1Max                    = 603;      //  Physically max value of TPS1 903
const uint16_t value2Min                    = 919;      //  Physically min value of TPS2 919
const uint16_t value2Idle                   = 914;      //  Idle position of TPS2 914
const uint16_t value2Max                    = 419;      //  Physically max value of TPS2 119
uint8_t implausibility;
}

namespace time 
{
const unsigned long tpsInterval             = 100;
const unsigned long blipDurationInterval    = 100;
const unsigned long blipLastInterval        = 1000;
unsigned long tps, blip, blipLast;
}
