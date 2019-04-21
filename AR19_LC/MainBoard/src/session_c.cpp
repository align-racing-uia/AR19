#include "session.h"


access::access(ErrorHandler* EH, uint16_t componentID) : 
_EH(EH), _componentID(componentID)
{
    _currentUser = -1;
    numerOfUsers = 0;
    newUser(0xEE, 0x45);
    newUser(0x33, 0x32);
    newUser(0x11, 0x45);
    newUser(0xAA, 0xBB);

}

void access::newUser(uint8_t userName, uint8_t password){

    if (numerOfUsers < 10){

        users[numerOfUsers].userName = userName;
        users[numerOfUsers].password = password;
        users[numerOfUsers].userID = numerOfUsers;
        users[numerOfUsers].logedIn = false;
        numerOfUsers ++;

    }
}

void access::logIn(uint8_t userName, uint8_t password){
    for(uint8_t ii = 0; ii < numerOfUsers; ii++)
    {
        if (users[ii].userName == userName && users[ii].password == password){

            users[ii].logedIn = true;
            _currentUser = users[ii].userID;
            _EH->newError(_componentID, 310 + users[ii].userID);
        }
        else
        {
            users[ii].logedIn = false;
            _currentUser = -1;
        }
        
    }
}

void access::logOut(uint8_t userName){
    for(uint8_t ii = 0; ii < numerOfUsers; ii++)
    {
        if (users[ii].userName == userName){

            users[ii].logedIn = false;
            _EH->newError(_componentID, 320 + users[ii].userID);
            _currentUser = -1;

        }
    }
}

bool access::isLogedIn(){

    return users[_currentUser].logedIn;

}

uint8_t access::getCurrentUserID(){

    return _currentUser;

}
