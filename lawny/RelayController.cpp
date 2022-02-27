#include "RelayController.hpp"

RelayController::RelayController() {
    pinMode(POWER_PIN_1, OUTPUT);
    pinMode(POWER_PIN_2, OUTPUT);
    pinMode(MOTOR_CONTROLLER_BLADE_1, OUTPUT);
    pinMode(MOTOR_CONTROLLER_BLADE_2, OUTPUT);
    pinMode(DRIVE_MOTOR, OUTPUT);

    //Set to high since active low
    digitalWrite(POWER_PIN_1, 1);
    digitalWrite(POWER_PIN_2, 1);
    digitalWrite(MOTOR_CONTROLLER_BLADE_1, 1);
    digitalWrite(MOTOR_CONTROLLER_BLADE_2, 1);
    digitalWrite(DRIVE_MOTOR, 1);
}

RelayController::~RelayController() {
    digitalWrite(POWER_PIN_1, 1);
    digitalWrite(POWER_PIN_2, 1);
    digitalWrite(MOTOR_CONTROLLER_BLADE_1, 1);
    digitalWrite(MOTOR_CONTROLLER_BLADE_2, 1);
    digitalWrite(DRIVE_MOTOR, 1);
}

void RelayController::setPower(int set) {
    digitalWrite(POWER_PIN_1, set);
    digitalWrite(POWER_PIN_2, set);
}

void RelayController::setBladeMotor(int set) {
    digitalWrite(MOTOR_CONTROLLER_BLADE_1, set);
    digitalWrite(MOTOR_CONTROLLER_BLADE_2, set);
}
        
void RelayController::setDriverMotor(int set) {
    digitalWrite(DRIVE_MOTOR, set);
}

void RelayController::setFullRelay(int set) {
    RelayController::setPower(set);
    RelayController::setBladeMotor(set);
    RelayController::setDriverMotor(set);
}
