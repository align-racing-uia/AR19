//  Controls which mode Pedalbox controller is running. Defaul=false
bool calibrateMode = false;

//  Defines global true and false used for CAN msg
const uint8_t globalTrue  = 0xF0;
const uint8_t globalFalse = 0x0F;

//  Pedalbox ACM ID
const uint16_t acmId = 0x012;

//  Definition of CAN ID's
const uint16_t canIdCalibrateToggle           = 0x0F0;
const uint16_t canIdAcmCheck                  = 0x019;
const uint16_t canIdApps                      = 0x011;
const uint16_t canIdBps                       = 0x410;
const uint16_t canIdRpm                       = 0x5F0;
const uint16_t canIdPedalboxImplausibility    = 0x010;

//  Definitions of pedalbox fault flag codes
const uint8_t faultCodeApps1OutOfRangeMin           = 41;
const uint8_t faultCodeApps2OutOfRangeMin           = 42;
const uint8_t faultCodeAppsBothOutOfRangeMin        = 40;
const uint8_t faultCodeApps1OutOfRangeMax           = 61;
const uint8_t faultCodeApps2OutOfRangeMax           = 62;
const uint8_t faultCodeAppsBothOutOfRangeMax        = 60;
const uint8_t faultCodeAppsDifferenceAboveTenPct    = 50;
const uint8_t faultCodeBps1OutOfRangeMin            = 141;
const uint8_t faultCodeBps2OutOfRangeMin            = 142;
const uint8_t faultCodeBpsBothOutOfRangeMin         = 140;
const uint8_t faultCodeBps1OutOfRangeMax            = 161;
const uint8_t faultCodeBps2OutOfRangeMax            = 162;
const uint8_t faultCodeBpsBothOutOfRangeMax         = 160;
const uint8_t faultCodeBpsDifferenceAboveTenPct     = 150;

//  Definition of APPS related variables
uint16_t apps1Value;
uint16_t apps2Value;
uint16_t apps1Out;
uint16_t apps2Out;
int16_t appsDifference;
uint8_t appsDifference_percent;
uint8_t apps1Pin = 17;
uint8_t apps2Pin = 16;
const uint16_t apps1Min = 264;  //  265
const uint16_t apps1Max = 696;  //  656
const uint16_t apps2Min = 184;  //  194
const uint16_t apps2Max = 412;  //  404

//  Variables used for checking implausibility for APPS
uint8_t appsImplausibilityOutOfRangeMargin  = 10;
bool apps2ImplausibilityOutofRangeMax       = false;
bool apps2ImplausibilityOutofRangeMin       = false;
bool apps1ImplausibilityOutofRangeMax       = false;
bool apps1ImplausibilityOutofRangeMin       = false;
uint8_t appsImplausible                     = 0;
unsigned long appsLastDiffImplausibility_ms = 0;

//  Definition of BPS related variables
uint16_t bps1Value;
uint16_t bps2Value;
uint16_t bps1Out;
uint16_t bps2Out;
int16_t bpsDifference;
uint8_t bpsDifference_percent;
uint8_t bps1Pin = 18;
uint8_t bps2Pin = 19;
const uint16_t bps1Min = 92;
const uint16_t bps1Max = 888;
const uint16_t bps2Min = 84;
const uint16_t bps2Max = 680;

//  Variables used for checking implausibility for BPS
uint8_t bpsImplausibilityOutOfRangeMargin   = 30;
bool bps2ImplausibilityOutofRangeMax        = false;
bool bps2ImplausibilityOutofRangeMin        = false;
bool bps1ImplausibilityOutofRangeMax        = false;
bool bps1ImplausibilityOutofRangeMin        = false;
uint8_t bpsImplausible                      = 0;
unsigned long bpsLastDiffImplausibility_ms  = 0;

//  Time variables used for CAN msg intervals
const unsigned long appsInterval_ms                     = 10;
unsigned long appsTimestampLastMsg_ms                   = 0;
const unsigned long bpsInterval_ms                      = 100;
unsigned long bpsTimestampLastMsg_ms                    = 0;
const unsigned long calibrateInterval_ms                = 100;
unsigned long calibrateTimestampLastMsg_ms              = 0;
const unsigned long rpmInterval_ms                      = 10;
unsigned long rpmTimestampLastMsg_ms                    = 0;
const unsigned long appsImplausibilityInterval_ms       = 100;
unsigned long appsImplausibilityLastMsg_ms              = 0;
const unsigned long bpsImplausibilityInterval_ms        = 500;
unsigned long bpsImplausibilityLastMsg_ms               = 0;
