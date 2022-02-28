#include "MotorBladeModule.hpp"
#include "CommunicationInterface.hpp"

#define DEBUG

static char on_off_in[3];

enum BLADE_CMD_ENUM {OFF, ON, INVALID};

static inline BLADE_CMD_ENUM cmd_to_enum(const char* s) {
    if(strcmp(s, "OFF") == 0) return BLADE_CMD_ENUM::OFF;
    if(strcmp(s, "ON") == 0) return BLADE_CMD_ENUM::ON;
    return BLADE_CMD_ENUM::INVALID;
}

MotorBladeModule::~MotorBladeModule() {
    analogWrite(BLADE_PWM, 0);
}

void MotorBladeModule::init() {
    pinMode(BLADE_DIR, OUTPUT);
    pinMode(BLADE_PWM, OUTPUT);
    digitalWrite(BLADE_DIR, 1);
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
    sscanf(cmd, "%s", &on_off_in);
    BLADE_CMD_ENUM on_off = cmd_to_enum(on_off_in);
    switch(on_off) {
        case ON:
            startBlade();
            #ifdef DEBUG 
            Serial.println("Blade Started");
            #endif
            break;
        case OFF:
            stopBlade();
            #ifdef DEBUG 
            Serial.println("Blade Stopped");
            #endif
            break;
        default:
            CommunicationInterface::writeErrorToSerial(moduleName, String("Blade Motor"), "Blade Motor did not get valid on or off command");
            break;
    }
}