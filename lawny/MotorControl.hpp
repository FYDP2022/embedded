#ifndef MOTOR_CONTROL_D
#define MOTOR_CONTROL_D
#include <Arduino.h>
#include <stdio.h>

#define GEAR_RATIO 51
#define PULSE_COUNT 6
#define CM_PER_REV 47
#define PULSE_PER_REV ( GEAR_RATIO ) * ( PULSE_COUNT )

#define PULSE_PER_CM 6.375

// Pin Assignments
#define LEFT_MOTOR_ENCODER 18 // Interupt Pin
#define RIGHT_MOTOR_ENCODER 19 // Interupt Pin
#define LEFT_PWM 44 // PWM Frequency connected to Timer 1
#define RIGHT_PWM 45 // PWM Frequency connected to Timer 1
#define LEFT_DIR 22 // Generic Digital Output
#define RIGHT_DIR 24 // Generic Digital Output

#define CW 0x0
#define CCW 0x1

#define MAX_PWM 200
#define MIN_PWM 70
#define MAX_BIAS 20

#define DRIVE_WIDTH_CM 34
#define DRIVE_RADI 17
#define CM_LARGE_CIRCLE 107L
#define CM_SMALL_CIRCLE 53L

enum CMD_ENUM {FORWARD, REVERSE, STOP, POINT_LEFT, POINT_RIGHT, FWD_LEFT, FWD_RIGHT, BWD_LEFT, BWD_RIGHT, INVALID};

class MotorController {
    public:
        MotorController();
        ~MotorController();
        void init();
        
        void update_state();
        
        int parse_command(const char* cmd);
        void stop_moving();    
        
    private:
        void forward(int speed, int bias, int cm);
        void reverse(int speed, int bias, int cm);
        void stop_left();
        void stop_right();
        void point_left(int speed, int bias, int degrees);
        void point_right(int speed, int bias, int degrees);
        void fwd_left(int speed, int bias, int degrees);
        void fwd_right(int speed, int bias, int degrees);
        void bwd_left(int speed, int bias, int degrees);
        void bwd_right(int speed, int bias, int degrees);

        void check_encoder_state();
        void set_encoder_targets(int LCOUNT, int RCOUNT);
        void set_motor_speeds(int L_PWM, int L_DIR, int R_PWM, int R_DIR);

        int current_left_pwm;
        int current_right_pwm;

        int desired_LCOUNT;
        int desired_RCOUNT;
        String moduleName = "MotorController"; 
};

extern MotorController wheel_motors;

#endif