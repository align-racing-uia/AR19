void variableClutchEngage (uint16_t clutchSpeed)
{


using namespace variableclutch;



if (millis() - timeStampDeclining > clutchSpeed/90 && clutchpressure::InBar > 1)
        {
            servoDegree = servoDegree - 1;
            servo.write(servoDegree);
            timeStampDeclining = millis();
        }

else
{
    servoDegree = 90;
}
}
