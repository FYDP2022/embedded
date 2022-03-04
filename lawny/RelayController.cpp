#include "RelayController.hpp"
#include "CommunicationInterface.hpp"

#define DEBUG

static char on_off_in[3];

enum RELAY_CMD_ENUM {OFF, ON, INVALID};

static inline RELAY_CMD_ENUM cmd_to_enum(const char* s) {
    if(strcmp(s, "OFF") == 0) return RELAY_CMD_ENUM::OFF;
    if(strcmp(s, "ON") == 0) return RELAY_CMD_ENUM::ON;
    return RELAY_CMD_ENUM::INVALID;
}

RelayController::~RelayController() {
    digitalWrite(POWER_PIN_1, 1);
    digitalWrite(POWER_PIN_2, 1);
    digitalWrite(MOTOR_CONTROLLER_BLADE_1, 1);
    digitalWrite(MOTOR_CONTROLLER_BLADE_2, 1);
    digitalWrite(DRIVE_MOTOR, 1);
}

bool RelayController::init() {
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
    return true;
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
    sscanf(cmd, "%s", &on_off_in);
    RELAY_CMD_ENUM on_off = cmd_to_enum(on_off_in);
    switch(on_off) {
        case ON:
            setFullRelay(0);
            #ifdef DEBUG
            Serial.println("Relay Started");
            #endif
            break;
        case OFF:
            setFullRelay(1);
            #ifdef DEBUG
            Serial.println("Relay Stopped");
            #endif
            break;
        default:
            CommunicationInterface::writeErrorToSerial(moduleName, "serial_error", "Relay did not get valid on or off command");
            break;
    }
}
