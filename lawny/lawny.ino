#include "CommunicationInterface.hpp"

CommunicationInterface interface = CommunicationInterface();

void setup() {
  Serial.begin(115200);
}


//Will probably have to add code for calling modules in the loop
void loop() {
  interface.parseCommand();
  Serial.print(interface.sendInputString());
}
