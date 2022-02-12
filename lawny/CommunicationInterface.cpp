#include "CommunicationInterface.hpp"

CommunicationInterface::CommunicationInterface() {
    stringComplete = false;
    inputString.reserve(200);
}

CommunicationInterface::~CommunicationInterface() {
    return;
}

bool CommunicationInterface::isCommandAvailable() {
    return Serial.available();
}