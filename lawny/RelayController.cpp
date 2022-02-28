#include "RelayController.hpp"
#include "CommunicationInterface.hpp"

static char cmd_in[5];
static char on_off_in[3];

static inline RELAY_CMD_ENUM cmd_to_enum(const char* s) {
    if(strcmp(s, "BLADE") == 0) return RELAY_CMD_ENUM::BLADE;
    if(strcmp(s, "ALL") == 0) return RELAY_CMD_ENUM::ALL;
    if(strcmp(s, "OFF") == 0) return RELAY_CMD_ENUM::OFF;
    if(strcmp(s, "ON") == 0) return RELAY_CMD_ENUM::ON;
    return RELAY_CMD_ENUM::INVALID;
}


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
    memset(cmd_in, 0, 5);
    int tokens = sscanf(cmd, "%[^:]:%d", &cmd_in, &on_off_in);
    if(tokens == 2) {
        RELAY_CMD_ENUM c = cmd_to_enum(cmd_in);
        RELAY_CMD_ENUM on_off = cmd_to_enum(on_off_in);
        switch(c) {
            case ALL:
                if(on_off == RELAY_CMD_ENUM::ON) {
                    setFullRelay(0);
                } 
                else if (on_off == RELAY_CMD_ENUM::OFF) {
                    setFullRelay(1);
                }
                break;
            
            default:
                break;
        }
    }
    d:

    // if (relay_part == 'a') {
    //     //o for on
    //     if (on_off == 'o') {
    //         setBladeMotor(0);
    //     //f for off
    //     } else if (on_off == 'f') {
    //         setBladeMotor(1);
    //     } else {
    //         CommunicationInterface::writeErrorToSerial(moduleName, String("Full Relay"), "Full Relay did not get valid on or off command");
    //     }
    // }
    // //b for blade
    // else if (relay_part == 'b') {
    //     //o for on
    //     if (on_off == 'o') {
    //         setPower(0);
    //     //f for off
    //     } else if (on_off == 'f') {
    //         setPower(1);
    //     } else {
    //         CommunicationInterface::writeErrorToSerial(moduleName, String("Blade Relay"), "Blade Relay did not get valid on or off command");
    //     }
    // } else {
    //     CommunicationInterface::writeErrorToSerial(moduleName, String("Relay command"), "Received invalid Relay command");
    // }
}