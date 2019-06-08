bool calibrateMode          = false;
bool launchControlMode      = false;
bool blipMode               = false;
bool limpMode               = false;
bool regulatedIdleEnable    = false;

uint8_t throttleSetpointMap = 0;

bool pwmSuccess1            = false;
bool pwmSuccess2            = false;
bool bootSuccess            = false;

uint8_t globalTrue          = 0xF0;
uint8_t globalFalse         = 0x0F;

uint16_t acmId  = 0x016;

uint16_t pwmFrequency   = 20000;

uint16_t canIdShutdown                  = 0x001;
uint16_t canIdCalibrateToggle           = 0x0F0;
uint16_t canIdAcmCheck                  = 0x019;
uint16_t canIdApps                      = 0x011;
uint16_t canIdPedalboxImplausible       = 0x010;
uint16_t canIdTps                       = 0x460;
uint16_t canIdTpsImplausible            = 0x009;
uint16_t canIdRpm                       = 0x5F0;
uint16_t canIdLcRequest                 = 0x290;
uint16_t canIdLcConfirm                 = 0x2B0;
uint16_t canIdBlipRequest               = 0x020;
uint16_t canIdBlipConfirm               = 0x2C0;
uint16_t canIdEtbcLimpModeConfirm       = 0x470;
uint16_t canIdLimpModeToggle            = 0x471;
uint16_t canIdChangeIdleRpm             = 0x472;
uint16_t canIdChangeIdleRpmConfirm      = 0x473;
uint16_t canIdChangeIdleValue           = 0x474;
uint16_t canIdChangeIdleValueConfirm    = 0x475;

uint8_t etbcOut1Pin     = 10;
uint8_t etbcOut2Pin     = 3;
uint8_t etbcEnablePin   = 11;
uint8_t etbcDisablePin  = 12;
uint8_t etbcFeedbackPin = 19;

uint8_t tps1Pin = 16;
uint8_t tps2Pin = 17;
uint16_t tps1Value;
uint16_t tps2Value;
uint8_t tps1Out;
uint8_t tps2Out;
int16_t tpsDifference;
uint8_t tpsDifference_percent;

uint8_t tps1ImplausibilityOutofRange;
uint8_t tps2ImplausibilityOutofRange;
uint8_t tpsImplausibilityDifference;
uint8_t tpsImplausible                      = 0;
unsigned long tpsLastDiffImplausibility_ms  = 0;

const uint16_t tps1Limp = 164;
const uint16_t tps1Min  = 158;   //  88 uten bspd / 158 med bspd
uint16_t tps1Idle       = 19   >>>>ZSA0;  //  164 uten bspd / 220 med bspd
const uint16_t tps1Max  = 670;  //  780 uten bspd / 720 med bspd
const uint16_t tps2Min  = 834;  //  834 uten bspd / 834 med bspd
uint16_t tps2Idle       = 760;  //  740 uten bspd / 740 med bspd
const uint16_t tps2Max  = 230;  //  230 uten bspd / 230 med bspd

uint8_t apps1Remap;
uint8_t apps1Value;
uint8_t apps2Value;
uint8_t appsDifference_percent;

uint16_t rpmValue;
uint16_t rpmIdle    = 1700;

uint8_t lcTargetValue;

bool reachLimpCheckStarted                  = false;
bool limpReachedOk                          = false;
const unsigned long reachLimpInterval_ms    = 1000;
unsigned long limpModeTimer_ms              = 0;

const unsigned long tpsInterval_ms                  = 100;
unsigned long tpsTimestampLastMsg_ms                = 0;
const unsigned long calibrateInterval_ms            = 100;
unsigned long calibrateTimestampLastMsg_ms          = 0;
const unsigned long blipDuration_ms                 = 100;
const unsigned long blipIntervalLimit_ms            = 1000;
unsigned long blipTimestampLastMsg_ms               = 0;
const unsigned long tpsImplausibilityInterval_ms    = 100;
unsigned long tpsImplausibilityLastMsg_ms           = 0;

double pidEtbInput, pidEtbSetpoint, pidEtbOutput;
double pidEtbOutputLimitMin         = -250;
double pidEtbOutputLimitMax         =  250;
unsigned long pidEtbSampleTime_us   = 1;
double pidEtbKp                     = 7.00;
double pidEtbKi                     = 3.00;
double pidEtbKd                     = 0.00;

double pidIdleInput, pidIdleSetpoint, pidIdleOutput;
double pidIdleOutputLimitMin        = tps1Min+5;
double pidIdleOutputLimitMax        = tps1Min+200;
unsigned long pidIdleSampleTime_us  = 10;
double pidIdleKp                    = 2.00;
double pidIdleKi                    = 1.00;
double pidIdleKd                    = 0.00;
