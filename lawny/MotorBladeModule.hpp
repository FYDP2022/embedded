#include <Arduino.h>

#define BLADE_DUTY_CYCLE 25

#define BLADE_PWM 5 // PWM Frequency connected to Timer 1
#define BLADE_DIR 4 // Generic Digital Output

class MotorBladeModule {
    public:
        MotorBladeModule();
        ~MotorBladeModule();
        bool init();
        
        void stopBlade();
        void parkingBrake();
        void update_state(bool isFlat);
        void parse_command(const char* cmd);

    private:
        void startBladeWithPWM(int duty_cycle);
        void startBlade();
        String moduleName = "MotorBladeModule";
};