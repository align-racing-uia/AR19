
void NeutralToFirst(){


    using namespace neutraltofirst;

    if (cansignal::engineRPM > gearing::softStartRPM &&  millis()-timeStamp > timeOut )
    {
        timeStamp =  millis();
    }

    if (millis() - timeStamp < timer )
    {

        if(gearposition::currentGear == 0)
        {
            servo.write(clutch::disengage); // Sets the servo to 90 degrees position
        }

        if(clutchpressure::InBar >= clutchpressure::disengagedThreshold && gearposition::currentGear == 0)
        {
            digitalWrite(gearing::downPin, HIGH);
        }
        
        if (gearposition::currentGear == 1 && clutchpressure::InBar > 1 && millis() - timeStamp < preEngaging)
        {
            digitalWrite(gearing::downPin, LOW);
            variableClutchEngage(engageSpeed);
        }
        else if (gearposition::currentGear == 1 && clutchpressure::InBar > 1 && millis() - timeStamp > preEngaging)
        {
            cansignal::neutralSignal = global::sant;
        }
    else if (millis() - timeStamp < timeOut && cansignal::neutralSignal == global::tull)
    {
        servo.write(clutch::engage);
        variableclutch::servoDegree = 90;
        digitalWrite(gearing::downPin, LOW);
        digitalWrite(gearing::upPin, LOW);
    }
}
