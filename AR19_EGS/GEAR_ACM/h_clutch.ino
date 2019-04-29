
void ClutchIn(){
    
    
    using namespace clutch; //Clutch override sensor må leggast in.
        if (millis() - clutch::timestamp < clutch::duration) // Signal from CAN that clutch has to be activated
    {
     servo.write(disengage); // Sets the servo to 90 degrees position
    }
    else 
    {
     servo.write(engage); // Sets the servo to 0 degrees position
    }

}


