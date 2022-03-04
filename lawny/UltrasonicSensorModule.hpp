#include <Arduino.h>
#include "NewPing.h"
#include "MotorControl.hpp"

#define MAX_DISTANCE 50
#define DANGER_DISTANCE 10

#define SENSOR_FRONT_ECHO 26
#define SENSOR_FRONT_TRIGGER 27
#define SENSOR_FRONT_LEFT_ECHO 28
#define SENSOR_FRONT_LEFT_TRIGGER 29
#define SENSOR_LEFT_ECHO 30
#define SENSOR_LEFT_TRIGGER 31
#define SENSOR_BACK_LEFT_ECHO 32
#define SENSOR_BACK_LEFT_TRIGGER 33
#define SENSOR_BACK_ECHO 34  // Back not used
#define SENSOR_BACK_TRIGGER 35 // Back not used
#define SENSOR_BACK_RIGHT_ECHO 36
#define SENSOR_BACK_RIGHT_TRIGGER 37
#define SENSOR_RIGHT_ECHO 40
#define SENSOR_RIGHT_TRIGGER 41
#define SENSOR_FRONT_RIGHT_ECHO 42
#define SENSOR_FRONT_RIGHT_TRIGGER 43

enum directionDesignation {
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
        int update_state();
        int isSafeUltraSonic(DRIVE_CMD_ENUM c);
        int checkDangerReading(directionDesignation direction);
        
    private:
        //index 0 is front, index 2 is left, index 4 is back, index 6 is right
        unsigned int sensorReadings [8];
        unsigned int sensor_index = 0;
        String moduleName = "UltrasonicSensorModule";
        
        NewPing sonarArr[8] = {
            NewPing(SENSOR_FRONT_TRIGGER, SENSOR_FRONT_ECHO, MAX_DISTANCE),
            NewPing(SENSOR_FRONT_LEFT_TRIGGER, SENSOR_FRONT_LEFT_ECHO, MAX_DISTANCE),
            NewPing(SENSOR_LEFT_TRIGGER, SENSOR_LEFT_ECHO, MAX_DISTANCE),
            NewPing(SENSOR_BACK_LEFT_TRIGGER, SENSOR_BACK_LEFT_ECHO, MAX_DISTANCE),
            NewPing(SENSOR_BACK_TRIGGER, SENSOR_BACK_ECHO, MAX_DISTANCE),
            NewPing(SENSOR_BACK_RIGHT_TRIGGER, SENSOR_BACK_RIGHT_ECHO, MAX_DISTANCE),
            NewPing(SENSOR_RIGHT_TRIGGER, SENSOR_RIGHT_ECHO, MAX_DISTANCE),
            NewPing(SENSOR_FRONT_RIGHT_TRIGGER, SENSOR_FRONT_RIGHT_ECHO, MAX_DISTANCE)
        };
};

extern UltrasonicSensorModule ultrasonic;
