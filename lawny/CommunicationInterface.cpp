#include "CommunicationInterface.hpp"

CommunicationInterface::CommunicationInterface() {
    inputString.reserve(200);
}

CommunicationInterface::~CommunicationInterface() {
    return;
}

char CommunicationInterface::parseCommand(char* cmdInput) {
    inputString = Serial.readStringUntil('\n');
    char controller_opt = inputString[0];
    inputString = inputString.substring(2);

    inputString.toCharArray(cmdInput, 50);
    inputString = "";

    return controller_opt;
    //testing code
    return 'F';
}

int CommunicationInterface::writeErrorToSerial(String module, String errorType, String errorMsg) {
    int written = Serial.println(String("error") + ":" + module + ":" + errorType + ":" + errorMsg);
    return written;
}

int CommunicationInterface::writeSensorDataToSerial(String module, String SpecificSensor, int Reading) {
    int written = Serial.println(String("sensor_data") + ":" + module + ":" + SpecificSensor + ":" + Reading);
    return written;
}
