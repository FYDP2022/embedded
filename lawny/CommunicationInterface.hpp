#ifndef CommunicationInterface_D
#define CommunicationInterface_D
#include <Arduino.h>

class CommunicationInterface {
    public:
        CommunicationInterface();
        ~CommunicationInterface();         
        int isCommandAvailable();
        char parseCommand();
        String sendInputString();
        int writeToSerial(String msg);
        void updateState();

        
    private:
        bool stringComplete;
        String inputString;
};
#endif
