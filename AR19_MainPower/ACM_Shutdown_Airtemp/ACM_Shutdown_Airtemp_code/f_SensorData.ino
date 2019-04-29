


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CheckShutdownStatus() {
using namespace shutdowncircuit;
  sensorValue = analogRead(SensePin);

  if(sensorValue < global::threshold)
  {
    cansignal::shutdownActive = global::sant;
  }
  else
  {
    cansignal::shutdownActive = global::tull;
  }
    
 
} 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CheckAirTempAmbient() {
  using namespace ambient;
  airTempSensorValue = analogRead(airTempPin);
  calculator = 0.000000534484754*airTempSensorValue*airTempSensorValue*airTempSensorValue - 0.000729604246401*airTempSensorValue*airTempSensorValue + 0.385815612455726*airTempSensorValue - 60.80769672243649; //Sjå dokumentasjon for transfer funksjon //her må inn transfer funksjon
  cansignal::ambientAirTemp = calculator;
  }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CheckAirTempTurbo() {
  using namespace turbo;
    airTempSensorValue = analogRead(airTempPin);
    calculator = 0.000000534484754*airTempSensorValue*airTempSensorValue*airTempSensorValue - 0.000729604246401*airTempSensorValue*airTempSensorValue + 0.385815612455726*airTempSensorValue - 60.80769672243649; //Sjå dokumentasjon for transfer funksjon //her må inn transfer funksjon
    cansignal::turboDischargeAirTemp = calculator;
  }
