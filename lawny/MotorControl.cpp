#include "MotorControl.hpp"
#include "CommunicationInterface.hpp"
#include "UltrasonicSensorModule.hpp"
#include "GyroAccel.hpp"
//#define DEBUG

volatile long LCOUNT = 0;
volatile long RCOUNT = 0;
int current_left_dir = 0;
int current_right_dir = 0;

static char cmd_in[12];
int speed_in;
int bias_in;
int distance_cm_in;

static inline DRIVE_CMD_ENUM cmd_to_enum(const char* s) {
    if(strcmp(s, "FORWARD") == 0) return DRIVE_CMD_ENUM::FORWARD;
    if(strcmp(s, "REVERSE") == 0) return DRIVE_CMD_ENUM::REVERSE;
    if(strcmp(s, "STOP") == 0) return DRIVE_CMD_ENUM::STOP;
    if(strcmp(s, "POINT_LEFT") == 0) return DRIVE_CMD_ENUM::POINT_LEFT;
    if(strcmp(s, "POINT_RIGHT") == 0) return DRIVE_CMD_ENUM::POINT_RIGHT;
    if(strcmp(s, "FWD_LEFT") == 0) return DRIVE_CMD_ENUM::FWD_LEFT;
    if(strcmp(s, "FWD_RIGHT") == 0) return DRIVE_CMD_ENUM::FWD_RIGHT;
    if(strcmp(s, "BWD_LEFT") == 0) return DRIVE_CMD_ENUM::BWD_LEFT;
    if(strcmp(s, "BWD_RIGHT") == 0) return DRIVE_CMD_ENUM::BWD_RIGHT;
    return DRIVE_CMD_ENUM::INVALID;

}

inline int cm_to_pulse_count(int cm) {
    return  (int)((cm * PULSE_PER_CM) + 0.5);
}

void MOTOR_COUNTER_ISR_L() {
    LCOUNT += (current_left_dir == CCW) ? 1 : -1;
}

void MOTOR_COUNTER_ISR_R() {
    RCOUNT += (current_right_dir == CW) ? 1 : -1;
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

bool MotorController::init() {
    TCCR5B = TCCR5B & B11111000 | B00000010; // set timer 5 divisor to 8 for PWM frequency of  3921.16 Hz (Pins 44, 45)
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
    pinMode(LEFT_MOTOR_ENCODER, INPUT_PULLUP);
    pinMode(RIGHT_MOTOR_ENCODER, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RIGHT_MOTOR_ENCODER), MOTOR_COUNTER_ISR_R, FALLING); // Encoder Pins
    attachInterrupt(digitalPinToInterrupt(LEFT_MOTOR_ENCODER), MOTOR_COUNTER_ISR_L, FALLING); // Encoder Pins
    sei(); // Re-enable interupts
    return true;
}

void MotorController::update_state() { // To be called very often
    check_encoder_state();
    // Check ultrasonic safety
    if(!ultrasonic.isSafeUltraSonic(current_state)) {
        stop_moving();
    }
    // Check gyroscope safety
    if(!gyroaccel.isFlat()) {
        stop_moving();
    }
}

int MotorController::parse_command(const char* cmd) {
    memset(cmd_in, 0, 12);
    speed_in = 0;
    bias_in = 0;
    distance_cm_in = 0;
    int tokens = sscanf(cmd, "%[^:]:%d:%d:%d", &cmd_in, &speed_in, &bias_in, &distance_cm_in);
    if(tokens == 4) {
        DRIVE_CMD_ENUM c = cmd_to_enum(cmd_in);
        if(ultrasonic.isSafeUltraSonic(c)) {
            switch(c) {
            case DRIVE_CMD_ENUM::FORWARD:
                forward(speed_in, bias_in, distance_cm_in);
                break;
            case DRIVE_CMD_ENUM::REVERSE:
                reverse(speed_in, bias_in, distance_cm_in);
                break;
            case DRIVE_CMD_ENUM::STOP:
                stop_moving();
                break;
            // For turning, "distance_cm_in" represents degrees instead of centimeters.
            case DRIVE_CMD_ENUM::POINT_LEFT:
                point_left(speed_in, bias_in, distance_cm_in);
                break;
            case DRIVE_CMD_ENUM::POINT_RIGHT:
                point_right(speed_in, bias_in, distance_cm_in);
                break;
            case DRIVE_CMD_ENUM::FWD_LEFT:
                fwd_left(speed_in, bias_in, distance_cm_in);
                break;
            case DRIVE_CMD_ENUM::FWD_RIGHT:
                fwd_right(speed_in, bias_in, distance_cm_in);
                break;
            case DRIVE_CMD_ENUM::BWD_LEFT:
                bwd_left(speed_in, bias_in, distance_cm_in);
                break;
            case DRIVE_CMD_ENUM::BWD_RIGHT:
                bwd_right(speed_in, bias_in, distance_cm_in);
                break;
            default:
                CommunicationInterface::writeErrorToSerial(moduleName, String("Wheel motor"), "Wheel motor did not get valid direction command");
                return -1;
            current_state = c;
            }
        }
        return 0;
    }
    CommunicationInterface::writeErrorToSerial(moduleName, String("Wheel motor"), "Wheel motor input command invalid length");
    return -1;
}

void MotorController::stop_moving() {
    stop_left();
    stop_right();
    set_encoder_targets(0,0);
    current_state = DRIVE_CMD_ENUM::STOP;
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
    #ifdef DEBUG
        Serial.print("STOPPED LEFT MOTOR\n");
    #endif
    analogWrite(LEFT_PWM, 0);
    current_left_pwm = 0;
    if(current_right_pwm == 0) {
        current_state = DRIVE_CMD_ENUM::STOP;
    }
}

void MotorController::stop_right() {
    #ifdef DEBUG
        Serial.print("STOPPED RIGHT MOTOR\n");
    #endif
    analogWrite(RIGHT_PWM, 0);
    current_right_pwm = 0;
    if(current_left_pwm == 0) {
        current_state = DRIVE_CMD_ENUM::STOP;
    }
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
    int l_speed = 0;
    int r_speed = ((speed + bias) * 255) / 100;
    if(r_speed < MIN_PWM) {
        r_speed = MIN_PWM;
    }
    int pulse_count = cm_to_pulse_count(((degrees * CM_LARGE_CIRCLE) / 360));
    set_encoder_targets(0, pulse_count);
    set_motor_speeds(l_speed, CW, r_speed, CW);
}

void MotorController::fwd_right(int speed, int bias, int degrees) {
    int l_speed = ((speed - bias) * 255) / 100;
    int r_speed = 0;
    if(l_speed < MIN_PWM) {
        l_speed = MIN_PWM;
    }
    int pulse_count = cm_to_pulse_count(((degrees * CM_LARGE_CIRCLE) / 360));
    set_encoder_targets(pulse_count, 0);
    set_motor_speeds(l_speed, CCW, r_speed, CCW);
}

void MotorController::bwd_left(int speed, int bias, int degrees) {
    int l_speed = ((speed - bias) * 255) / 100;
    int r_speed = 0;
    if(l_speed < MIN_PWM) {
        l_speed = MIN_PWM;
    }
    int pulse_count = cm_to_pulse_count(((degrees * CM_LARGE_CIRCLE) / 360));
    set_encoder_targets(-pulse_count, 0);
    set_motor_speeds(l_speed, CW, r_speed, CW);
}

void MotorController::bwd_right(int speed, int bias, int degrees) {
    int l_speed = 0;;
    int r_speed = ((speed + bias) * 255) / 100;
    if(r_speed < MIN_PWM) {
        r_speed = MIN_PWM;
    }
    int pulse_count = cm_to_pulse_count(((degrees * CM_LARGE_CIRCLE) / 360));
    set_encoder_targets(0, -pulse_count);
    set_motor_speeds(l_speed, CCW, r_speed, CCW);
}

void MotorController::check_encoder_state() {
    if(current_left_pwm != 0) {
        if(current_left_dir == CCW && LCOUNT > desired_LCOUNT) {
            stop_left();
        }
        else if(current_left_dir == CW && LCOUNT < desired_LCOUNT) {
            stop_left();
        }
    }

    if(current_right_pwm != 0) {
        if(current_right_dir == CCW && RCOUNT < desired_RCOUNT) {
            stop_right();
        }
        else if(current_right_dir == CW && RCOUNT > desired_RCOUNT) {
            stop_right();
        }
    }
    // #ifdef DEBUG
    //     char x[32];
    //     sprintf(x, "L %d, R %d\n", LCOUNT, RCOUNT);
    //     Serial.print(x);
    // #endif
}

void MotorController::set_encoder_targets(int D_LCOUNT, int D_RCOUNT) {
    desired_LCOUNT = LCOUNT + D_LCOUNT;
    desired_RCOUNT = RCOUNT + D_RCOUNT;
    #ifdef DEBUG 
        Serial.print("Left Target,");
        Serial.print(desired_LCOUNT);
        Serial.print(" Right Target,");
        Serial.print(desired_RCOUNT);
        Serial.print("\n");
    #endif
}

void MotorController::set_motor_speeds(int L_PWM, int L_DIR, int R_PWM, int R_DIR) {
    analogWrite(LEFT_PWM, L_PWM); // Set motors to PWM stop
    analogWrite(RIGHT_PWM, R_PWM); // Set motors to PWM stop
    current_left_pwm = L_PWM;
    current_right_pwm = R_PWM;
    // If we are stopping, don't change the direction pin or the motor will "lock"
    if(L_PWM != 0) {
        digitalWrite(LEFT_DIR, L_DIR); 
    }
    if(R_PWM != 0) {
        digitalWrite(RIGHT_DIR, R_DIR);
    }
    current_left_dir = L_DIR;
    current_right_dir = R_DIR;
}
