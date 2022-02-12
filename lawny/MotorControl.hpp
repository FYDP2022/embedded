#ifndef MOTOR_CONTROL_D
#define MOTOR_CONTROL_D
#include <Arduino.h>

// Pin Assignments
#define LEFT_PWM A0
#define RIGHT_PWM A1
#define LEFT_DIR 51
#define RIGHT_DIR 49
//#define LEFT_FEEDBACK X
//#define RIGHT_FEEDBACK Y

class MotorController {
    public:
        MotorController();
        ~MotorController();
        int set_motor_speeds(byte L_PWM, byte L_DIR, byte R_PWM, byte R_DIR);
    private:
        byte current_left_pwm;
        byte current_right_pwm;
        byte current_left_dir;
        byte current_right_dir;
};



#endif