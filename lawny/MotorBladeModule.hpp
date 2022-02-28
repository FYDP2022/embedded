#include <Arduino.h>

#define BLADE_DUTY_CYCLE 25

#define BLADE_PWM 5 // PWM Frequency connected to Timer 1
#define BLADE_DIR 4 // Generic Digital Output

class MotorBladeModule {
    public:
        ~MotorBladeModule();
        void init();
        void startBladeWithPWM(int duty_cycle);
        void startBlade();
        void stopBlade();
        void parkingBreak();
        void MotorBladeModule::update_state(bool isFlat);
        void parse_command(const char* cmd);

    private:
        String moduleName = "MotorBladeModule";
};