#include "CommunicationInterface.hpp"

CommunicationInterface::CommunicationInterface() {
    stringComplete = false;
    inputString.reserve(200);
}

CommunicationInterface::~CommunicationInterface() {
    return;
}

int CommunicationInterface::isCommandAvailable() {
    return Serial.available();
}

String CommunicationInterface::parseCommand() {
    if (isCommandAvailable() > 0) {
        inputString = Serial.readStringUntil('\n');
        int delim = inputString.indexOf(':');
        String moduleSubstring = inputString.substring(0,delim);
        return moduleSubstring;
    }
    //testing code
    return "noMsg";
}

int CommunicationInterface::writeToSerial(String msg) {
    int written = Serial.print(msg);
    return written;
}
