void GatherSensorData()
{
  CheckClutchPressure(); 
  CheckGear();
}



void CheckClutchPressure() 
{
  using namespace clutchpressure;
  sensorValue = analogRead(sensorPin);
  InBar = map(sensorValue, 100, 921, 0, 250); // Remaps the bit value to conincide with BAR - info in datasheet //Henrik: Mest sansynlig så bør innputsignalet mappes fra 0 til 255

  // Giver error code to the telemetry if clutch pressure is out of range.
  if (sensorValue < 90 || sensorValue > 921)
  {
    cansignal::clutchPressureError = global::sant;
  }
  else
  {
    cansignal::clutchPressureError = global::tull;
  }
} 



void CheckGear()
{
  using namespace gearposition;

// 3.1.1 Local code - obtaining gearpositionsensor data and converting to mV

  sensorInput = analogRead(sensorPin);
  voltage = map (sensorInput, 0, 1023, 0, 5000);

// 3.1.2 Gear voltage calculation, with +/- 100mV margin 
/*
  1st :	  648	    mV
  N   :  	1 040	  mV
  2nd :	  1 429	  mV
  3rd :	  2 188	  mV
  4th :	  2 957	  mV
  5th :	  3 688	  mV
  6th :	  4 420	  mV

*/

if (voltage < 400)
{
  currentGear = 69;
}

else if ( voltage > 548 && voltage < 748 )
{
  currentGear = 1;
}

else if (voltage > 900 && voltage < 1150 )
{
  currentGear = 0;           // NB! Gear position "0" is Netutral(N)
}

else if (voltage > 1329 && voltage < 1529 )
{
  currentGear = 2;
}

else if (voltage > 2088 && voltage < 2288 )
{
  currentGear = 3;
}

else if (voltage > 2857 && voltage < 3057)
{
  currentGear = 4;
}

else if (voltage > 3588 && voltage < 3788 )
{
  currentGear = 5;
}

else if (voltage > 4320 && voltage < 4520 )
{
  currentGear = 6;
}

else if (voltage > 4600)
{
  currentGear = 69;
}

else 
{
  currentGear = 99;        // NB! Gear position "99" is no gear, and may be reached between gears. This signal in accordance with some timing, can be used to debuging and errorcode
}

// Sets telemetry value if gear position sensor is out of range
if (currentGear == 69)
{
  cansignal::gearPositionError = global::sant;
}
else
{
  cansignal::gearPositionError = global::tull;
}


}
