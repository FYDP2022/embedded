#include "MotorBladeModule.hpp"

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

void startBlade() {
    analogWrite(BLADE_PWM, BLADE_DUTY_CYCLE);
}
        
void stopBlade() {
    analogWrite(BLADE_PWM, 0);
}