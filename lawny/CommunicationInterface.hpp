#ifndef CommunicationInterface_D
#define CommunicationInterface_D
#include <Arduino.h>

class CommunicationInterface {
    public:
        CommunicationInterface();
        ~CommunicationInterface();         
        int isCommandAvailable();
        char parseCommand(char* cmdInput);
        static int writeErrorToSerial(String Module, String ErrType, String ErrorMsg);
        static int writeSensorDataToSerial(String Module, String SpecificSensor, int Reading);
        
    private:
        String inputString;
        String moduleName = "CommunicationInterface";
};
#endif
