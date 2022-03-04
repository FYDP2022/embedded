#include "MotorBladeModule.hpp"
#include "CommunicationInterface.hpp"

#define DEBUG

static char on_off_in[3];

enum class BLADE_CMD_ENUM {OFF, ON, INVALID};

static inline BLADE_CMD_ENUM cmd_to_enum(const char* s) {
    if(strcmp(s, "OFF") == 0) return BLADE_CMD_ENUM::OFF;
    if(strcmp(s, "ON") == 0) return BLADE_CMD_ENUM::ON;
    return BLADE_CMD_ENUM::INVALID;
}

MotorBladeModule::MotorBladeModule() {
    analogWrite(BLADE_PWM, 0);
}

MotorBladeModule::~MotorBladeModule() {
    analogWrite(BLADE_PWM, 0);
}

bool MotorBladeModule::init() {
    pinMode(BLADE_DIR, OUTPUT);
    pinMode(BLADE_PWM, OUTPUT);
    digitalWrite(BLADE_DIR, 1);
    analogWrite(BLADE_PWM, 0);
    return true;
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

void MotorBladeModule::update_state(bool isFlat) {
    if (!isFlat) {
        CommunicationInterface::writeErrorToSerial(moduleName, "state_error", "lawny_not_flat");
        stopBlade();
    }
}

void MotorBladeModule::parse_command(const char* cmd) {
    sscanf(cmd, "%s", &on_off_in);
    BLADE_CMD_ENUM on_off = cmd_to_enum(on_off_in);
    switch(on_off) {
        case BLADE_CMD_ENUM::ON:
            startBlade();
            #ifdef DEBUG 
            Serial.println("Blade Started");
            #endif
            break;
        case BLADE_CMD_ENUM::OFF:
            stopBlade();
            #ifdef DEBUG 
            Serial.println("Blade Stopped");
            #endif
            break;
        default:
            CommunicationInterface::writeErrorToSerial(moduleName, "serial_error", "Blade Motor did not get valid on or off command");
            break;
    }
}