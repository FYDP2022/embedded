#include "MotorBladeModule.hpp"
#include "CommunicationInterface.hpp"

MotorBladeModule::MotorBladeModule() {
    pinMode(BLADE_DIR, OUTPUT);
    pinMode(BLADE_PWM, OUTPUT);
    digitalWrite(BLADE_DIR, 1);
    analogWrite(BLADE_PWM, 0);
}

MotorBladeModule::~MotorBladeModule() {
    analogWrite(BLADE_PWM, 0);
}

void MotorBladeModule::startBladeWithPWM(int duty_cycle) {
    analogWrite(BLADE_PWM, duty_cycle);
}

void MotorBladeModule::startBlade() {
    analogWrite(BLADE_PWM, BLADE_DUTY_CYCLE);
}
        
void MotorBladeModule::stopBlade() {
    analogWrite(BLADE_PWM, 0);
}

void MotorBladeModule::parse_command(const char* cmd) {
    //o for on
    if (cmd[0] == 'o') {
        startBlade();
    //f for off
    } else if (cmd[0] == 'f') {
        stopBlade();
    } else {
        CommunicationInterface::writeErrorToSerial(moduleName, String("Blade Motor"), "Blade Motor did not get valid on or off command");
    }
}