//  Controls which mode ETC is running. Default=false
bool calibrateMode          = false;

//  Variables to check for succesful boot and setup
bool pwmSuccess1            = false;
bool pwmSuccess2            = false;
bool bootSuccess            = false;

//  Defines the global true and false values used for CAN msg
const uint8_t globalTrue          = 0xF0;
const uint8_t globalFalse         = 0x0F;

//  EGS ACM ID
const uint16_t acmId  = 0x017;

//  Definitions of CAN ID's
const uint16_t canIdShutdown                    = 0x001;    //  ID for shutdown signal to shutdown ACM
const uint16_t canIdCalibrateToggle             = 0x0F0;    //  ID for toggling calibration mode
const uint16_t canIdAcmCheck                    = 0x019;    //  ID for ACM ping and status report
const uint16_t canIdEgsFaultFlag                = 0x480;    //  ID for fault codes in EGS error state

//  Definitions of EGS fault flag codes
const uint8_t faultCodeThrottlePositionNotReached   = 10;


//  Definition of ACM pins controlling H-bridge to ETB
const uint8_t etbcOut1Pin     = 10;
const uint8_t etbcOut2Pin     = 3;
const uint8_t etbcEnablePin   = 11;
const uint8_t etbcDisablePin  = 12;
const uint8_t etbcFeedbackPin = 19;

//  Definition of TPS related variables
const uint8_t tps1Pin = 16;
const uint8_t tps2Pin = 17;
uint16_t tps1Value;
uint16_t tps2Value;
uint8_t tps1Out;
uint8_t tps2Out;
int16_t tpsDifference;
uint8_t tpsDifference_percent;

//  Variables used for implausibility checks
bool tps2ImplausibilityOutofRangeMax        = false;
bool tps2ImplausibilityOutofRangeMin        = false;
bool tps1ImplausibilityOutofRangeMax        = false;
bool tps1ImplausibilityOutofRangeMin        = false;
uint8_t tpsImplausible                      = 0;
unsigned long tpsLastDiffImplausibility_ms  = 0;

//  Variables used in comparing target with actual position
const unsigned long reachThrottleTargetLimit_ms = 500;
unsigned long reachThrottleTargetTimer_ms       = 1000;

//  Max and min values for TPS1 and 2
const uint16_t tps1Limp         = 296;  //  296 med BSPD
const uint16_t tps1Min          = 192;  //  192 med BSPD
const uint16_t tps1Max          = 864;  //  864 med BSPD
uint16_t tps1Idle               = 255;  //  230 med BSPD, Økte til 255 for å øke idle til 9 % (lest av i Tunerstudio) 
uint16_t tps1IdleOld            = tps1Idle;  //  old idle value when enabling/disabling auto idle
const uint16_t tps2Min          = 838;  //  838 med BSPD
const uint16_t tps2Max          = 156;  //  156 med BSPD
const uint16_t tps1PhysicalMax  = 972;  //  Mechanical stop with BSPD
const uint16_t tps2PhysicalMax  = 48;   //  Mechanical stop with BSPD

//  Variables used for mapping throttle target
uint8_t apps1Remap;
uint8_t apps1Value;
uint8_t apps2Value;
uint8_t appsDifference_percent;

//  Variables used in auto idle from RPM. Default=1700 RPM
uint16_t rpmValue;
uint16_t rpmIdle    = 1700;

// Variable to set setpoint from LC 
uint8_t lcTargetValue;

//  Variables used in check for reaching limp mode 
bool limpStarted                            = false;
bool throttleInLimp                         = false;
const unsigned long reachLimpInterval_ms    = 1000;
unsigned long limpModeTimer_ms              = 1000;

//  Time variables used for CAN msg intervals 
const unsigned long tpsInterval_ms                  = 100;
unsigned long tpsTimestampLastMsg_ms                = 0;
const unsigned long calibrateInterval_ms            = 100;
unsigned long calibrateTimestampLastMsg_ms          = 0;
const unsigned long blipDuration_ms                 = 100;
const unsigned long blipIntervalLimit_ms            = 1000;
unsigned long blipTimestampLastMsg_ms               = 0;
const unsigned long tpsImplausibilityInterval_ms    = 100;
unsigned long tpsImplausibilityLastMsg_ms           = 0;
const unsigned long limpMsgInterval_ms              = 100;
unsigned long lastLimpMsg_ms                        = 0;