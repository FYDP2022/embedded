#include <Arduino.h>

#define BLADE_DUTY_CYCLE 25

#define POWER_PIN_1 50
#define POWER_PIN_2 51
#define MOTOR_CONTROLLER_BLADE_1 52
#define MOTOR_CONTROLLER_BLADE_2 53
#define DRIVE_MOTOR 54


class RelayController {
    public:
        ~RelayController();
        //set either 1 or 0 (0 for on, 1 for off)
        void init();
        void setPower(int set);
        void setBladeMotor(int set);
        void setDriverMotor(int set);
        void setFullRelay(int set);
        void parse_command(const char* cmd);

    private:
        String moduleName = "RelayController";
};