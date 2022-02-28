#include "RelayController.hpp"
#include "CommunicationInterface.hpp"

char cmd_in[3];

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

void RelayController::parse_command(const char* cmd) {
    char relay_part;
    char on_off;
    Serial.println(cmd);
    int tokens = sscanf(cmd, "%c:%c", &relay_part, &on_off);
    //b for blade
    Serial.println(String("RELAY COMMAND: ") + relay_part + on_off);
    if (relay_part == 'b') {
        //o for on
        if (on_off == 'o') {
            setBladeMotor(0);
        //f for off
        } else if (on_off == 'f') {
            setBladeMotor(1);
        } else {
            CommunicationInterface::writeErrorToSerial(moduleName, String("Full Relay"), "Full Relay did not get valid on or off command");
        }
    }
    //a for all
    else if (relay_part == 'a') {
        //o for on
        if (on_off == 'o') {
            setPower(0);
        //f for off
        } else if (on_off == 'f') {
            setPower(1);
        } else {
            CommunicationInterface::writeErrorToSerial(moduleName, String("Blade Relay"), "Blade Relay did not get valid on or off command");
        }
    } else {
        CommunicationInterface::writeErrorToSerial(moduleName, String("Relay command"), "Received invalid Relay command");
    }
}
