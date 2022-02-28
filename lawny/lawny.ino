#include <Arduino.h>
#include "CommunicationInterface.hpp"
#include "UltrasonicSensorModule.hpp"
#include "Temperature.hpp"
#include "MotorControl.hpp"
#include "GyroAccel.hpp"

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

char cmd_string[50];

//Unused for Now.
CommunicationInterface interface = CommunicationInterface();
GyroAccel gyroaccel = GyroAccel();
TemperatureSensor temp_sensor = TemperatureSensor();

void setup() {
    Serial.begin(115200);
    gyroaccel.init();
    motor_controller.init();
    temp_sensor.init();
}


//Will probably have to add code for calling modules in the loop
void loop() {
    if(stringComplete) {

        motor_controller.parse_command(inputString.c_str());
        stringComplete = false;
        inputString = "";
    }
    motor_controller.update_state();
    //gyroaccel.update_state();
    temp_sensor.update_state();
}



void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
