#include "UltrasonicSensorModule.hpp"

UltrasonicSensorModule::UltrasonicSensorModule() {
    for (int i = 0; i < 8; i++) {
        UltrasonicSensorModule::sensorReadings[i] = 0;
    }
}

UltrasonicSensorModule::~UltrasonicSensorModule() {
    for (int i = 0; i < 8; i++) {
        UltrasonicSensorModule::sensorReadings[i] = 0;
    }
}

int UltrasonicSensorModule::updateUltrasonicReadings() {
    UltrasonicSensorModule::sensorReadings[0] = speedOfSoundConversion(sonar_front.ping_cm()); 
    UltrasonicSensorModule::sensorReadings[1] = speedOfSoundConversion(sonar_front_left.ping_cm()); 
    UltrasonicSensorModule::sensorReadings[2] = speedOfSoundConversion(sonar_left.ping_cm()); 
    UltrasonicSensorModule::sensorReadings[3] = speedOfSoundConversion(sonar_back_left.ping_cm()); 
    UltrasonicSensorModule::sensorReadings[4] = speedOfSoundConversion(sonar_back.ping_cm()); 
    UltrasonicSensorModule::sensorReadings[5] = speedOfSoundConversion(sonar_back_right.ping_cm()); 
    UltrasonicSensorModule::sensorReadings[6] = speedOfSoundConversion(sonar_right.ping_cm()); 
    UltrasonicSensorModule::sensorReadings[7] = speedOfSoundConversion(sonar_front_right.ping_cm());
}

int UltrasonicSensorModule::checkDangerReading(directionDesignation direction) {
    int index = convertDirectionToIndex(direction);
    if (UltrasonicSensorModule::sensorReadings[index] <= DANGER_DISTANCE) {
        return -1;
        Serial.println("Sensor " + convertDirectionToName(direction) + " detected we are too close to an obstacle");
    }
    return 0;
}

int UltrasonicSensorModule::readUltrasonicSensor(CMD_ENUM c) {
    int error_desig = 0;
    switch(c) {
        case FORWARD:
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::F);
            break;
        case REVERSE:
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::B);
            break;
        case STOP:
            break;
        case POINT_LEFT:
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::L);
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::FL);
            break;
        case POINT_RIGHT:
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::R);
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::FR);
            break;
        case FWD_LEFT:
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::L);
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::FL);
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::F);
            break;
        case FWD_RIGHT:
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::R);
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::FR);
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::F);
            break;
        case BWD_LEFT:
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::L);
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::BL);
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::B);
            break;
        case BWD_RIGHT:
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::R);
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::BR);
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::B);
            break;
    return error_desig;
}