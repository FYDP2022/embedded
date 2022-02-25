#include <Arduino.h>
#include "NewPing.h"
#include "MotorControl.hpp"

#define MAX_DISTANCE 50
#define DANGER_DISTANCE 10

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

class UltrasonicSensorModule {
    public:
        UltrasonicSensorModule();
        ~UltrasonicSensorModule();  
        int updateUltrasonicReadings();
        int readUltrasonicSensor(CMD_ENUM c);
        int checkDangerReading(directionDesignation direction);
        
    private:
        //index 0 is front, index 2 is left, index 4 is back, index 6 is right
        unsigned int sensorReadings [8];
};
