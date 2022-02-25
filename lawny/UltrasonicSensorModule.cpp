#pragma once

#include "UltrasonicSensorModule.hpp"

NewPing sonar_front(sensor_front_trigger, sensor_front_echo, MAX_DISTANCE);
NewPing sonar_front_left(sensor_front_left_trigger, sensor_front_left_echo, MAX_DISTANCE);
NewPing sonar_left(sensor_left_trigger, sensor_left_echo, MAX_DISTANCE);
NewPing sonar_back_left(sensor_back_left_trigger, sensor_back_left_echo, MAX_DISTANCE);
NewPing sonar_back(sensor_back_trigger, sensor_back_echo, MAX_DISTANCE);
NewPing sonar_back_right(sensor_back_right_trigger, sensor_back_right_echo, MAX_DISTANCE);
NewPing sonar_right(sensor_right_trigger, sensor_right_echo, MAX_DISTANCE);
NewPing sonar_front_right(sensor_front_right_trigger, sensor_front_right_echo, MAX_DISTANCE);

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

UltrasonicSensorModule::UltrasonicSensorModule() {
    for (int i = 0; i < 8; i++) {
        UltrasonicSensorModule::sensorReadings[i] = 0;
    }
    //pinMode(sensor_front_trigger, OUTPUT);
    //pinMode(sensor_front_echo, INPUT);
}

UltrasonicSensorModule::~UltrasonicSensorModule() {
    for (int i = 0; i < 8; i++) {
        UltrasonicSensorModule::sensorReadings[i] = 0;
    }
}

int UltrasonicSensorModule::updateUltrasonicReadings() {
    UltrasonicSensorModule::sensorReadings[0] = sonar_front.ping_cm(); 
    UltrasonicSensorModule::sensorReadings[1] = sonar_front_left.ping_cm(); 
    UltrasonicSensorModule::sensorReadings[2] = sonar_left.ping_cm(); 
    UltrasonicSensorModule::sensorReadings[3] = sonar_back_left.ping_cm(); 
    UltrasonicSensorModule::sensorReadings[4] = sonar_back.ping_cm(); 
    UltrasonicSensorModule::sensorReadings[5] = sonar_back_right.ping_cm(); 
    UltrasonicSensorModule::sensorReadings[6] = sonar_right.ping_cm(); 
    UltrasonicSensorModule::sensorReadings[7] = sonar_front_right.ping_cm();
}

int UltrasonicSensorModule::checkDangerReading(directionDesignation direction) {
    int index = convertDirectionToIndex(direction);
    int distance = UltrasonicSensorModule::sensorReadings[index];
    if (distance <= DANGER_DISTANCE && distance != 0) {
        // TODO: Use communication interface to throw all errors.
        Serial.println("Sensor " + convertDirectionToName(direction) + " detected we are too close to an obstacle");
        return -1;
    }
    Serial.println(convertDirectionToName(direction) + " " + distance);
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
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::BL);
            break;
        case POINT_RIGHT:
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::R);
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::FR);
            error_desig += UltrasonicSensorModule::checkDangerReading(directionDesignation::BR);
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
    }
    return error_desig;
}
