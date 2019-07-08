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
const uint16_t canIdShutdown            = 0x001;    //  ID for shutdown signal to shutdown ACM
const uint16_t canIdCalibrateToggle     = 0x0F0;    //  ID for toggling calibration mode
const uint16_t canIdAcmCheck            = 0x019;    //  ID for ACM ping and status report
const uint16_t canIdEgsFaultFlag        = 0x480;    //  ID for fault codes in EGS error state
const uint16_t canIdEgsCalibrate        = 0x481;    //  ID for sending calibration values 
const uint16_t canIdRequestGearShift    = 0x240;    //  ID for recieving gear shift requests
const uint16_t canIdRequestNeutral      = 0x042;    //  ID for recieving neutral requests
const uint16_t canIdEgsOut              = 0x021;    //  ID for transmitting gear position and clutch pressure

//  Definitions of EGS fault flag codes (available range: 0-255)
const uint8_t faultCodeInvalidGearShiftRequest  = 1;    //  Indicates error in gear shift request message recieved.
const uint8_t faultCodeShiftDownFailure         = 2;    //  Indicates error in shifting up
const uint8_t faultCodeShiftUpFailure           = 3;    //  Indicates error in shifting down
const uint8_t faultCodeShiftTimeout             = 4;    //  Indicates a shift request during shift timeout
const uint8_t faultCodeGearReadFailure          = 5;    //  Indictates an error in reading current gear
const uint8_t faultCodeShiftUpFromTopGear       = 6;    //  Indicates a request to gear up when in top gear
const uint8_t faultCodeShiftDownFromNeutral     = 7;    //  Indicates a request to gear down when in first gear

//  Definition of external pins
const uint8_t pinGearPositionSensor     = 18;
const uint8_t pinGearUp                 = 3;
const uint8_t pinGearDown               = 10;
const uint8_t pinClutchPressureSensor   = 19;

//  Definition of gear related variables
const uint16_t gearMarginValue      = 10;   //  Adjustable margin for calculating current gear
const uint16_t neutralGearValue     = 0;
const uint16_t firstGearValue       = 0;
const uint16_t secondGearValue      = 0;
const uint16_t thirdGearValue       = 0;
const uint16_t fourthGearValue      = 0;
const uint16_t fifthGearValue       = 0;
const uint16_t sixtGearValue        = 0;
uint16_t currentGear, gearSensorValue;
bool shiftTimeout                   = false;    //  Shift timeout to prevent excessive shifting

//  Definition of clutch related variables
uint16_t clutchPressure             = 0;

//  Time variables used for CAN msg intervals 
const unsigned long calibrateInterval_ms    = 100;  //  Interval [ms] for calibration messages
unsigned long calibrateTimestampLastMsg_ms  = 0;
const unsigned long egsOutInterval_ms       = 100;  //  Interval [ms] for EGS messages
unsigned long egsOutTimestampLastMsg_ms     = 0;

//  Time variables for other functions
const unsigned long shiftTimeoutInterval_ms = 100;  //  Interval [ms] until next gear shift is allowed
unsigned long shiftTimeoutLast_ms           = 0;
const unsigned long shiftUpInterval_ms      = 35;   //  Sets interval for how long the shifter is actuated at up shift.
unsigned long shiftUpTimer_ms               = 0;
const unsigned long shiftDownInterval_ms    = 35;   //  Sets interval for how long the shifter is actuated at down shift.
unsigned long shiftDownTimer_ms             = 0;
const unsigned long shiftNeutralInterval_ms = 15;   //  Sets interval for how long the shifter is actuated when shifting to neutral from first or second gear.
unsigned long shiftNeutralTimer_ms          = 0;
