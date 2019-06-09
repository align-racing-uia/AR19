void PingAcm(){
using namespace cansignal;

    //If any of the ACM does not send ping in return, the ping will replay every 1 second. 
    if (bitRead(acmOk, 1) == false || bitRead(acmOk, 2) == false || bitRead(acmOk, 3) == false || bitRead(acmOk, 4) == false || bitRead(acmOk, 5) == false  || bitRead(acmOk, 6) == false || bitRead(acmOk, 7) == false )
    {
        pingAllAcm = global::sant;
        bitWrite(acmOk,0,0);
    }
    else
    {
        pingAllAcm = global::tull;
        bitWrite(acmOk,0,1);
        shutdowncircuit::lockoutTimer = millis();
    }
}
