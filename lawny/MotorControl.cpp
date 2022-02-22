#include "MotorControl.hpp"

MotorController motor_controller;

volatile long MotorController::LCOUNT = 0;
volatile long MotorController::RCOUNT = 0;
int MotorController::current_left_dir = 0;
int MotorController::current_right_dir = 0;

char cmd_in[12];
int speed_in;
int bias_in;
int distance_cm_in;

enum CMD_ENUM {FORWARD, REVERSE, STOP, POINT_LEFT, POINT_RIGHT, FWD_LEFT, FWD_RIGHT, BWD_LEFT, BWD_RIGHT};

inline CMD_ENUM cmd_to_enum(char* s) {
    if(strcmp(s, "FORWARD") == 0) return CMD_ENUM::FORWARD;
    if(strcmp(s, "REVERSE") == 0) return CMD_ENUM::REVERSE;
    if(strcmp(s, "STOP") == 0) return CMD_ENUM::STOP;
    if(strcmp(s, "POINT_LEFT") == 0) return CMD_ENUM::POINT_LEFT;
    if(strcmp(s, "POINT_RIGHT") == 0) return CMD_ENUM::POINT_RIGHT;
    if(strcmp(s, "FWD_LEFT") == 0) return CMD_ENUM::FWD_LEFT;
    if(strcmp(s, "FWD_RIGHT") == 0) return CMD_ENUM::FWD_RIGHT;
    if(strcmp(s, "BWD_LEFT") == 0) return CMD_ENUM::BWD_LEFT;
    if(strcmp(s, "BWD_RIGHT") == 0) return CMD_ENUM::BWD_RIGHT;

}

inline int cm_to_pulse_count(int cm) {
    return  (int)((cm * PULSE_PER_CM) + 0.5);
}

void MotorController::MOTOR_COUNTER_ISR_L() {
    LCOUNT += (current_left_dir == CCW) ? 1 : 0;
}

void MotorController::MOTOR_COUNTER_ISR_R() {
    RCOUNT += (current_right_dir == CW) ? 1 : 0;
}

MotorController::MotorController() {
    current_left_pwm = 0;
    current_right_pwm = 0;
    current_left_dir = 0;
    current_right_dir = 0;
    RCOUNT = 0;
    LCOUNT = 0;
}

MotorController::~MotorController() {

}

void MotorController::init() {
    TCCR1B = TCCR1B & B11111000 | B00000010; // set timer 1 divisor to 8 for PWM frequency of  3921.16 Hz (Pins 11, 12)
    analogWrite(LEFT_PWM, 0); // Set motors to PWM stop
    analogWrite(RIGHT_PWM, 0); // Set motors to PWM stop
    pinMode(LEFT_DIR, OUTPUT); // Direction Pins
    pinMode(RIGHT_DIR, OUTPUT); // Direction Pins
    // Configure both motors to go forward by default.
    digitalWrite(LEFT_DIR, CCW); 
    digitalWrite(RIGHT_DIR, CW);
    current_left_dir = CCW;
    current_right_dir = CW;
    cli(); // Disable interupts to configure
    attachInterrupt(digitalPinToInterrupt(LEFT_MOTOR_ENCODER), MOTOR_COUNTER_ISR_L, RISING); // Encoder Pins
    attachInterrupt(digitalPinToInterrupt(RIGHT_MOTOR_ENCODER), MOTOR_COUNTER_ISR_R, RISING); // Encoder Pins
    sei(); // Re-enable interupts
}

void MotorController::update_state() { // To be called very often
    check_encoder_state();
}

int MotorController::parse_command(const char* cmd) {
    memset(cmd_in, 0, 12);
    speed_in = 0;
    bias_in = 0;
    distance_cm_in = 0;
    int tokens = sscanf(cmd, "%[^:]:%d:%d:%d", &cmd_in, &speed_in, &bias_in, &distance_cm_in);
    if(tokens == 4) {
        CMD_ENUM c = cmd_to_enum(cmd_in);
        switch(c) {
            case FORWARD:
                forward(speed_in, bias_in, distance_cm_in);
                break;
            case REVERSE:
                reverse(speed_in, bias_in, distance_cm_in);
                break;
            case STOP:
                stop_moving();
                break;
            case POINT_LEFT:
                point_left(speed_in, bias_in, distance_cm_in);
                break;
            case POINT_RIGHT:
                point_right(speed_in, bias_in, distance_cm_in);
                break;
            case FWD_LEFT:
                fwd_left(speed_in, bias_in, distance_cm_in);
                break;
            case FWD_RIGHT:
                fwd_right(speed_in, bias_in, distance_cm_in);
                break;
            case BWD_LEFT:
                bwd_left(speed_in, bias_in, distance_cm_in);
                break;
            case BWD_RIGHT:
                bwd_right(speed_in, bias_in, distance_cm_in);
                break;
        }
        return 0;
    }
    return -1;
}

void MotorController::stop_moving() {
    stop_left();
    stop_right();
    set_encoder_targets(0,0);
}

void MotorController::forward(int speed, int bias, int cm) {
    int pulse_count = cm_to_pulse_count(cm);
    int l_speed = ((speed - bias) * 255) / 100;
    int r_speed = ((speed + bias) * 255) / 100;
    if(l_speed < MIN_PWM) {
        r_speed += MIN_PWM - l_speed;
        l_speed = MIN_PWM;
    }
    if(r_speed < MIN_PWM) {
        l_speed += MIN_PWM - r_speed;
        r_speed = MIN_PWM;
    }
    set_encoder_targets(pulse_count, pulse_count);
    set_motor_speeds(l_speed, CCW, r_speed, CW);
}

void MotorController::reverse(int speed, int bias, int cm) {
    int pulse_count = cm_to_pulse_count(cm);
    int l_speed = ((speed - bias) * 255) / 100;
    int r_speed = ((speed + bias) * 255) / 100;
    if(l_speed < MIN_PWM) {
        r_speed += MIN_PWM - l_speed;
        l_speed = MIN_PWM;
    }
    if(r_speed < MIN_PWM) {
        l_speed += MIN_PWM - r_speed;
        r_speed = MIN_PWM;
    }
    pulse_count = -pulse_count;
    set_encoder_targets(pulse_count, pulse_count);
    set_motor_speeds(l_speed, CW, r_speed, CCW);
}

void MotorController::stop_left() {
    analogWrite(LEFT_PWM, 0);
    current_left_pwm = 0;
}

void MotorController::stop_right() {
    analogWrite(RIGHT_PWM, 0);
    current_right_pwm = 0;
}

void MotorController::point_left(int speed, int bias, int degrees) {
    int l_speed = ((speed - bias) * 255) / 100;
    int r_speed = ((speed + bias) * 255) / 100;
    if(l_speed < MIN_PWM) {
        r_speed += MIN_PWM - l_speed;
        l_speed = MIN_PWM;
    }
    if(r_speed < MIN_PWM) {
        l_speed += MIN_PWM - r_speed;
        r_speed = MIN_PWM;
    }
    int pulse_count = cm_to_pulse_count(((degrees * CM_SMALL_CIRCLE) / 360));
    set_encoder_targets(-pulse_count, pulse_count);
    set_motor_speeds(l_speed, CW, r_speed, CW);
}

void MotorController::point_right(int speed, int bias, int degrees) {
    int l_speed = ((speed - bias) * 255) / 100;
    int r_speed = ((speed + bias) * 255) / 100;
    if(l_speed < MIN_PWM) {
        r_speed += MIN_PWM - l_speed;
        l_speed = MIN_PWM;
    }
    if(r_speed < MIN_PWM) {
        l_speed += MIN_PWM - r_speed;
        r_speed = MIN_PWM;
    }
    int pulse_count = cm_to_pulse_count(((degrees * CM_SMALL_CIRCLE) / 360));
    set_encoder_targets(pulse_count, -pulse_count);
    set_motor_speeds(l_speed, CCW, r_speed, CCW);
}

void MotorController::fwd_left(int speed, int bias, int degrees) {

}

void MotorController::fwd_right(int speed, int bias, int degrees) {

}

void MotorController::bwd_left(int speed, int bias, int degrees) {

}

void MotorController::bwd_right(int speed, int bias, int degrees) {

}

void MotorController::check_encoder_state() {
    if(current_left_dir == CCW && LCOUNT < desired_LCOUNT) {
        stop_left();
    }
    else if(current_left_dir == CW && LCOUNT > desired_LCOUNT) {
        stop_left();
    }
    
    if(current_right_dir == CCW && RCOUNT < desired_RCOUNT) {
        stop_right();
    }
    else if(current_right_dir == CW && RCOUNT > desired_RCOUNT) {
        stop_right();
    }
}

void MotorController::set_encoder_targets(int D_LCOUNT, int D_RCOUNT) {
    desired_LCOUNT = LCOUNT + D_LCOUNT;
    desired_RCOUNT = RCOUNT + D_RCOUNT;
}

void MotorController::set_motor_speeds(int L_PWM, int L_DIR, int R_PWM, int R_DIR) {
    analogWrite(LEFT_PWM, L_PWM); // Set motors to PWM stop
    analogWrite(RIGHT_PWM, R_PWM); // Set motors to PWM stop
    current_left_pwm = L_PWM;
    current_right_pwm = R_PWM;
    digitalWrite(LEFT_DIR, L_DIR); 
    digitalWrite(RIGHT_DIR, R_DIR);
    current_left_dir = L_DIR;
    current_right_dir = R_DIR;
}
