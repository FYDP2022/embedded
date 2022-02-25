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

char CommunicationInterface::parseCommand() {
    if (isCommandAvailable() > 0) {
        inputString = Serial.readStringUntil('\n');
        char controller_opt = inputString[0];
        // TODO
        inputString = inputString.substring(1);
        return controller_opt;
    }
    //testing code
    return 'F';
}

String CommunicationInterface::sendInputString() {
    String copy = String(inputString);
    inputString = "";
    return copy;
}

int CommunicationInterface::writeToSerial(String msg) {
    int written = Serial.print(msg);
    return written;
}
