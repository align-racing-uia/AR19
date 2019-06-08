bool calibrateMode = false;
uint8_t globalTrue  = 0xF0;
uint8_t globalFalse = 0x0F;

uint16_t acmId = 0x012;

uint16_t canIdCalibrateToggle           = 0x0F0;
uint16_t canIdAcmCheck                  = 0x019;
uint16_t canIdApps                      = 0x011;
uint16_t canIdBps                       = 0x410;
uint16_t canIdRpm                       = 0x5F0;
uint16_t canIdPedalboxImplausibility    = 0x010;

uint16_t apps1Value;
uint16_t apps2Value;
uint16_t apps1Out;
uint16_t apps2Out;
int16_t appsDifference;
uint8_t appsDifference_percent;
uint8_t apps1Pin = 17;
uint8_t apps2Pin = 16;
const uint16_t apps1Min = 265;  //  265
const uint16_t apps1Max = 656;  //  656
const uint16_t apps2Min = 194;  //  194
const uint16_t apps2Max = 404;  //  404

uint16_t bps1Value;
uint16_t bps2Value;
uint16_t bps1Out;
uint16_t bps2Out;
int16_t bpsDifference;
uint8_t bpsDifference_percent;
uint8_t bps1Pin = 18;
uint8_t bps2Pin = 19;
const uint16_t bps1Min = 0;
const uint16_t bps1Max = 1023;
const uint16_t bps2Min = 0;
const uint16_t bps2Max = 1023;

uint8_t apps1ImplausibilityOutOfRange;
uint8_t apps2ImplausibilityOutOfRange;
uint8_t appsImplausibilityDifference;
uint8_t appsImplausible                             = 0;
unsigned long appsLastDiffImplausibility_ms         = 0;
const unsigned long appsImplausibilityInterval_ms   = 100;

uint8_t bps1ImplausibilityOutOfRange;
uint8_t bps2ImplausibilityOutOfRange;
uint8_t bpsImplausibilityDifference;
uint8_t bpsImplausible                              = 0;
unsigned long bpsLastDiffImplausibility_ms          = 0;
const unsigned long bpsImplausibilityInterval_ms    = 500;

uint8_t rpmPin  = 19;
uint16_t rpmValue;
uint8_t rpmOut1;
uint8_t rpmOut2;

const unsigned long appsInterval_ms                     = 10;
unsigned long appsTimestampLastMsg_ms                   = 0;
const unsigned long bpsInterval_ms                      = 100;
unsigned long bpsTimestampLastMsg_ms                    = 0;
const unsigned long calibrateInterval_ms                = 100;
unsigned long calibrateTimestampLastMsg_ms              = 0;
const unsigned long rpmInterval_ms                      = 10;
unsigned long rpmTimestampLastMsg_ms                    = 0;
const unsigned long pedalboxImplausibilityInterval_ms   = 100;
unsigned long pedalboxImplausibilityLastMsg_ms          = 0;
