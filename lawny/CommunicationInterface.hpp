#ifndef CommunicationInterface_D
#define CommunicationInterface_D
#include <Arduino.h>

class CommunicationInterface {
    public:
        CommunicationInterface();
        ~CommunicationInterface();         
        int isCommandAvailable();
        char parseCommand(char* cmdInput);
        int writeErrorToSerial(String Module, String ErrType, String ErrorMsg);
        int writeSensorDataToSerial(String Module, String SpecificSensor, int Reading);
        void updateState();

        
    private:
        String inputString;
        String moduleName = "CommunicationInterface";
};
#endif
