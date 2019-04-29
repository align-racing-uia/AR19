void GoToNeutral()
{

if ( millis() - gotoneutral::timestamp > gotoneutral::timer && gearposition::currentGear != 0)
{
    clutch::timestamp = millis();
           
    if (gearposition::currentGear >= 2)
    {
        digitalWrite(geardown::pin, HIGH);
        digitalWrite(gearup::pin, LOW);
    }
    else if (gearposition::currentGear == 1)
    {
        digitalWrite(gearup::pin, HIGH);
        digitalWrite(geardown::pin, LOW);
    }

}

else if( millis() - gotoneutral::timestamp > gotoneutral::timer && gearposition::currentGear == 0 && millis() - gotoneutral::timestamp < gotoneutral::timerLockout)
{
    servo.write(clutch::engage);
    digitalWrite(geardown::pin, LOW);
    digitalWrite(gearup::pin, LOW);
}

else if ( gearposition::currentGear != 0 && millis() - gotoneutral::timestamp > gotoneutral::timer && millis() - gotoneutral::timestamp < gotoneutral::timerLockout)
{
    digitalWrite(geardown::pin, LOW);
    digitalWrite(gearup::pin, LOW);
    servo.write(clutch::disengage);

}

}










































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
