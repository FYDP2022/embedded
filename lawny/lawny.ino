#include <Arduino.h>
#include "CommunicationInterface.hpp"
#include "UltrasonicSensorModule.hpp"
#include "Temperature.hpp"
#include "MotorControl.hpp"
#include "MotorBladeModule.hpp"
#include "RelayController.hpp"
#include "GyroAccel.hpp"

// Reset Function
void (* resetFunc) (void) = 0;

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
    //Wait until Serial is initialized
    while(!Serial);
    memset(cmd_string, 0, 50);
    bool err_cnt = 0;
    err_cnt |= relay.init();
    err_cnt |= gyroaccel.init();
    err_cnt |= wheel_motors.init();
    err_cnt |= temp_sensor.init();
    err_cnt |= blade_motor.init();
    if(err_cnt != 0) {
      interface.writeErrorToSerial(String("SETUP"), String("INIT"), String("failed initialization"));
    }
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
  blade_motor.update_state(gyroaccel.isFlat());
}
