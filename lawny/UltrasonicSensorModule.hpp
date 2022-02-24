#include <Arduino.h>
#include "NewPing.h"
#include "MotorControl.hpp"

#define MAX_DISTANCE 400
#define DANGER_DISTANCE 50

#define sensor_front_echo 54
#define sensor_front_trigger 55
#define sensor_front_left_echo 56
#define sensor_front_left_trigger 57
#define sensor_left_echo 58
#define sensor_left_trigger 59
#define sensor_back_left_echo 60
#define sensor_back_left_trigger 61
#define sensor_back_echo 62
#define sensor_back_trigger 63
#define sensor_back_right_echo 64
#define sensor_back_right_trigger 65
#define sensor_right_echo 66
#define sensor_right_trigger 67
#define sensor_front_right_echo 68
#define sensor_front_right_trigger 69

NewPing sonar_front(sensor_front_trigger, sensor_front_echo, MAX_DISTANCE);
NewPing sonar_front_left(sensor_front_left_trigger, sensor_front_left_echo, MAX_DISTANCE);
NewPing sonar_left(sensor_left_trigger, sensor_left_echo, MAX_DISTANCE);
NewPing sonar_back_left(sensor_back_left_trigger, sensor_back_left_echo, MAX_DISTANCE);
NewPing sonar_back(sensor_back_trigger, sensor_back_echo, MAX_DISTANCE);
NewPing sonar_back_right(sensor_back_right_trigger, sensor_back_right_echo, MAX_DISTANCE);
NewPing sonar_right(sensor_right_trigger, sensor_right_echo, MAX_DISTANCE);
NewPing sonar_front_right(sensor_front_right_trigger, sensor_front_right_echo, MAX_DISTANCE);

enum class directionDesignation {
    F,
    FL,
    L,
    BL,
    B,
    BR,
    R,
    FR
};

int convertDirectionToIndex(directionDesignation direction) {
    switch(direction) {
        case directionDesignation::F:       
            return 0;
        case directionDesignation::FL:
            return 1;
        case directionDesignation::L:
            return 2;
        case directionDesignation::BL:
            return 3;
        case directionDesignation::B:
            return 4;
        case directionDesignation::BR:
            return 5;
        case directionDesignation::R:
            return 6;
        case directionDesignation::FR:
            return 7;
    }
}

String convertDirectionToName(directionDesignation direction) {
    switch(direction) {
        case directionDesignation::F:       
            return "Front";
        case directionDesignation::FL:
            return "Front-Left";
        case directionDesignation::L:
            return "Left";
        case directionDesignation::BL:
            return "Back-Left";
        case directionDesignation::B:
            return "Back";
        case directionDesignation::BR:
            return "Back-Right";
        case directionDesignation::R:
            return "Right";
        case directionDesignation::FR:
            return "Front-Right";
    }
}
int speedOfSoundConversion(int duration) {
    return (duration / 2) * 0.0343;
}

class UltrasonicSensorModule {
    public:
        UltrasonicSensorModule();
        ~UltrasonicSensorModule();  
        int updateUltrasonicReadings();
        int readUltrasonicSensor(CMD_ENUM c);
        int checkDangerReading(directionDesignation direction);
        
    private:
        //index 0 is front, index 2 is left, index 4 is back, index 6 is right
        int sensorReadings [8];
};
