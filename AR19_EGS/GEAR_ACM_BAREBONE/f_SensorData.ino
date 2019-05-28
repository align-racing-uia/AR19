////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GatherSensorData()
{
  //CheckClutchPressure(); 
  //CheckGear();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CheckClutchPressure() 
{
using namespace clutchpressure;
    sensorValue = analogRead(sensorPin);
    InBar = map(sensorValue, 250, 902, 0, 100); // Remaps the bit value to conincide with BAR - info in datasheet

    // Giver error code to the telemetry if clutch pressure is out of range.
    if (sensorValue < 250 || sensorValue > 902)
    {
      cansignal::clutchPressureError = global::sant;
    }
    else
    {
      cansignal::clutchPressureError = global::tull;
    }
 

} 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CheckGear()
{
using namespace gearposition;

// 3.1.1 Local code - obtaining gearpositionsensor data and converting to mV

sensorInput = analogRead(inputPin);
voltage = map (sensorInput, 0, 1023, 0, 5000);

// 3.1.2 Gear voltage calculation, with +/- 50mV margin 

/*  Gear Voltage Data:
Gear:   Precise reading:        Range:
1st     565mV                   515mV - 615mV
N       1153mV                  1103mV - 1203mV
2nd     1383mV                  1333mV - 1433mV
3rd     2163mV                  2113mV - 2213mV
4th     3023mV                  2973mV - 3073mV
5th     3786mV                  3736mV - 3836mV
6th     4432mV                  4382mV - 4482mV
*/
    // 3.1.3 Deciding gear postion using bounderies

if ( voltage > 515 && voltage < 615 )
{
  currentGear = 1;
}

else if (voltage > 1103 && voltage < 1203 )
{
  currentGear = 0;           // NB! Gear position "0" is Netutral(N)
}

else if (voltage > 1333 && voltage < 1433 )
{
  currentGear = 2;
}

else if (voltage > 2113 && voltage < 2213 )
{
  currentGear = 3;
}

else if (voltage > 2973 && voltage < 3073 )
{
  currentGear = 4;
}

else if (voltage > 3736 && voltage < 3836 )
{
  currentGear = 5;
}

else if (voltage > 4382 && voltage < 4482 )
{
  currentGear = 6;
}

else 
{
  currentGear = 99;        // NB! Gear position "99" is no gear, and may be reached between gears. This signal in accordance with some timing, can be used to debuging and errorcode
}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////