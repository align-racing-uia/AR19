#ifndef session
#define session

#include "Arduino.h"
#include "errorHandler.h"





typedef struct user
{
    uint8_t userID;
    uint8_t userName;
    uint8_t password;
    bool logedIn;
};

class access
{
private:
    uint8_t numerOfUsers;
    user users[10];
    ErrorHandler* _EH;
    uint8_t _componentID;
    uint8_t _currentUser;


    void newUser(uint8_t userName, uint8_t password);

public:

    access(ErrorHandler* EH, uint16_t componentID);
    void logIn(uint8_t userName, uint8_t password);
    void logOut(uint8_t userName);
    uint8_t getCurrentUserID();

    bool isLogedIn();
    void innput(uint64_t data);
    
    
};




















#endif 