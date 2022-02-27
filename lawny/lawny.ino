#include <Arduino.h>
#include "CommunicationInterface.hpp"
#include "UltrasonicSensorModule.hpp"
#include "MotorControl.hpp"

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

char cmd_string[50];

//Unused for Now.
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
