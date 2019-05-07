
void ClutchIn(){
    
    
    using namespace clutch; //Clutch override sensor må leggast in.
        if (millis() - clutch::timestamp < clutch::duration && millis() > clutch::duration) // Signal from CAN that clutch has to be activated
    {
     servo.writeMicroseconds(disengage); //  Disconnects the engine and wheels
    }
    else 
    {
     servo.writeMicroseconds(engage); // Connects the engine and wheels
    }

}
