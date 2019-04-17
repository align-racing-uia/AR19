void PingAcm(){
using namespace cansignal;
    if (millis() - acmOkTimer < 1000)
    {
        //If any of the ACM does not send ping in return, the ping will replay every 1 second. 
        if (bitRead(acmOk, 0) == false || bitRead(acmOk, 1) == false || bitRead(acmOk, 2) == false || bitRead(acmOk, 3) == false || bitRead(acmOk, 4) == false || bitRead(acmOk, 5) == false  || bitRead(acmOk, 6) == false || bitRead(acmOk, 7) == false )
        {
            acmOkTimer = millis();
            pingAllAcm = global::sant;

        }
    else
    {
        pingAllAcm = global::tull;
    }
    }
}