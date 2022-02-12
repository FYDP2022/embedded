#ifndef CommunicationInterface_D
#define CommunicationInterface_D
#include <Arduino.h>

class CommunicationInterface {
    public:
        CommunicationInterface();
        ~CommunicationInterface();         
        bool isCommandAvailable();
        void updateState();
        
    private:
        bool stringComplete;
        String inputString;
};
#endif