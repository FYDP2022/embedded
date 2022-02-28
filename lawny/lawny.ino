#include <Arduino.h>
#include "CommunicationInterface.hpp"
#include "UltrasonicSensorModule.hpp"
#include "Temperature.hpp"
#include "MotorControl.hpp"
#include "MotorBladeModule.hpp"
#include "RelayController.hpp"
#include "GyroAccel.hpp"

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

char cmd_string[50];

//Unused for Now.
CommunicationInterface interface = CommunicationInterface();
UltrasonicSensorModule ultrasonic = UltrasonicSensorModule();
MotorController wheel_motors = MotorController();
MotorBladeModule blade_motor = MotorBladeModule();
RelayController relay = RelayController();
GyroAccel gyroaccel = GyroAccel();
TemperatureSensor temp_sensor = TemperatureSensor();

void setup() {
    Serial.begin(115200);
    memset(cmd_string, 0, 50);
    relay.init();
    gyroaccel.init();
    wheel_motors.init();
    temp_sensor.init();
    blade_motor.init();
}


//Will probably have to add code for calling modules in the loop
void loop() {
    if (Serial.available()) {
    char module = interface.parseCommand(cmd_string);
    switch (toUpperCase(module)) {
      //Wheel Motors
      case 'W':
        wheel_motors.parse_command(cmd_string);
        break;
      
      //Relay
      case 'R':
        relay.parse_command(cmd_string);
        break;
      
      //Blade Motors
      case 'B':
        blade_motor.parse_command(cmd_string);
        break;
    }
    memset(cmd_string, 0, 50);
  }
  wheel_motors.update_state();
  gyroaccel.update_state();
  temp_sensor.update_state();
  ultrasonic.update_state();
}
