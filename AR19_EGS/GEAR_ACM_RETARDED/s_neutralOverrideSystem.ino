

if ( gearposition::currentGear != 0 && clutchpressure::InBar < clutchpressure::disengagedTreshold && millis() - gearing::timerGearDown > gearing::gearTimerTimeout && millis() - gearing::timerGearUp > gearing::gearTimerTimeout && millis() - gotoneutral::timerNeutralTimeout > gotoneutral::neutralTimeout + gotoneutral::safeTime )
{






}

