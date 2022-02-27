#include "CommunicationInterface.hpp"

CommunicationInterface interface = CommunicationInterface();
char cmdInput [20];

void setup() {
  Serial.begin(115200);
  memset(cmdInput, 0, 20);
}


//Will probably have to add code for calling modules in the loop
void loop() {
  while(interface.isCommandAvailable() > 0) {
    char module = interface.parseCommand(cmdInput);
    Serial.println(cmdInput);
    switch (tolower(module)) {
      //Wheel Motors
      case 'w':
        //motorController parseCommand method, passing cmdInput
        break;
      
      //Relay Motors
      case 'r':
        //Relay Motors parseCommand method, passing cmdInput
        break;
      
      //Blade Motors
      case 'b':
        //bladeController parseCommand method, passing cmdInput
        break;
    }

  }
  memset(cmdInput, 0, 20);
}
