#include <Arduino.h>
#include "CommunicationInterface.hpp"
#include "UltrasonicSensorModule.hpp"
#include "MotorControl.hpp"
#include "MotorBladeModule.hpp"
#include "RelayController.hpp"

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

char cmd_string[50];

//Unused for Now.
CommunicationInterface interface = CommunicationInterface();
UltrasonicSensorModule ultrasonic = UltrasonicSensorModule();
MotorController wheel_motors = MotorController();
MotorBladeModule blade_motor = MotorBladeModule();
RelayController relay = RelayController();

void setup() {
  Serial.begin(115200);
  memset(cmd_string, 0, 50);
}


//Will probably have to add code for calling modules in the loop
void loop() {
  while(interface.isCommandAvailable() > 0) {
    char module = interface.parseCommand(cmd_string);
    Serial.println(cmd_string);
    switch (tolower(module)) {
      //Wheel Motors
      case 'w':
        wheel_motors.parse_command(cmd_string);
        break;
      
      //Relay
      case 'r':
        relay.parse_command(cmd_string);
        break;
      
      //Blade Motors
      case 'b':
        blade_motor.parse_command(cmd_string);
        break;
    }

  }
  memset(cmd_string, 0, 50);
}
