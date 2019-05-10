void GoToNeutral()
{

using namespace gotoneutral;

if(cansignal::neutralSignal == global::sant && millis() - timerNeutralTimeout > neutralTimeout + safeTime && millis()- gearing::timerGearUp > gearing::gearTimerTimeout  && millis() - gearing::timerGearDown > gearing::gearTimerTimeout && gearposition::currentGear != 0)
{
    timerNeutralTimeout = millis();
    shiftTimer = millis();
}


if ( millis() - timerNeutralTimeout > neutralTimeout && gearposition::currentGear != 0)
{
    servo.write(clutch::disengage);
    
    if (millis() - shiftTimer < shiftAttemptLength )
    {
        
        if (gearposition::currentGear >= 2 && gearposition::currentGear <= 6)
        {
            digitalWrite(gearing::downPin, HIGH);
        }
        else if (gearposition::currentGear == 1)
        {
            digitalWrite(gearing::upPin, HIGH);
        }
    }
    else if (millis() - shiftTimer > shiftAttemptLength && millis() - shiftTimer < shiftAttemptLength + shiftingPause  )
    {
        digitalWrite(gearing::downPin, LOW);
        digitalWrite(gearing::upPin, LOW);
    }
    else if(millis() - shiftTimer > shiftAttemptLength + shiftingPause)
    {
        shiftTimer = millis();
    }
}

else if( millis() - timerNeutralTimeout > neutralTimeout && gearposition::currentGear == 0)
{
    servo.write(clutch::engage);
    digitalWrite(gearing::downPin, LOW);
    digitalWrite(gearing::upPin, LOW);
}

else if ( gearposition::currentGear != 0 && millis() - timerNeutralTimeout > neutralTimeout + safeTime )
{
    digitalWrite(gearing::downPin, LOW);
    digitalWrite(gearing::upPin, LOW);
    servo.write(clutch::disengage);

}


cansignal::neutralSignal = global::tull;


}

// Har ikke laget ferdig spesialtilfeller og feilkilder, skal jobbe med det fremover











































/*
if(cansignal::neutralSignal == global::sant && gearposition::currentGear != 0 && millis() - gearing::timerNeutralTimeout < gearing::neutralTimeout)
{
    clutch::timestamp = millis();
    gearing::timerNeutral = millis();
}
else {cansignal::neutralSignal = global::tull;}


if(cansignal::neutralSignal == global::sant && gearposition::currentGear > 2){
        
        cansignal::gearDownSignal = global::sant;    
}


if(cansignal::neutralSignal == global::sant && gearposition::currentGear == 2){
        
    while (millis() - gearing::timerNeutral < gearing::gearTimerNeutral)
    {
        pinMode(gearing::downPin, HIGH);
    }
    pinMode(gearing::downPin, LOW);
        
}

if(cansignal::neutralSignal == global::sant && gearposition::currentGear == 1){
        
    while (millis() - gearing::timerNeutral < gearing::gearTimerNeutral)
    {
        pinMode(gearing::downPin, LOW);
    }
    pinMode(gearing::downPin, HIGH);
        
}

}
*/
