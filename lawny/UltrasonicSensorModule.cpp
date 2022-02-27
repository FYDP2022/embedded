#pragma once

#include "UltrasonicSensorModule.hpp"
#include "CommunicationInterface.hpp"

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
        sensorReadings[i] = 0;
    }
    
}

UltrasonicSensorModule::~UltrasonicSensorModule() {
    for (int i = 0; i < 8; i++) {
        sensorReadings[i] = 0;
    }
}

int UltrasonicSensorModule::updateUltrasonicReadings() {
    String sensor_name = convertDirectionToName((directionDesignation) sensor_index);
    sensorReadings[sensor_index] = sonarArr[sensor_index].ping_cm();
    CommunicationInterface::writeSensorDataToSerial(moduleName, sensor_name, sensorReadings[sensor_index]);
    sensor_index = (sensor_index + 1) % 8;
}

int UltrasonicSensorModule::checkDangerReading(directionDesignation direction) {
    int index = (int) direction;
    int distance = sensorReadings[index];
    if (distance <= DANGER_DISTANCE && distance != 0) {
        String sensor_name = convertDirectionToName((directionDesignation) sensor_index);
        CommunicationInterface::writeErrorToSerial(moduleName, String("Ultrasonic sensor ") + sensor_name, "Sensor detected to close to obstacle");
        return -1;
    }
    Serial.println(convertDirectionToName(direction) + " " + distance);
    return 0;
}

int UltrasonicSensorModule::readUltrasonicSensor(CMD_ENUM c) {
    int error_desig = 0;
    switch(c) {
        case FORWARD:
            error_desig += checkDangerReading(directionDesignation::F);
            break;
        case REVERSE:
            error_desig += checkDangerReading(directionDesignation::B);
            break;
        case STOP:
            break;
        case POINT_LEFT:
            error_desig += checkDangerReading(directionDesignation::L);
            error_desig += checkDangerReading(directionDesignation::FL);
            error_desig += checkDangerReading(directionDesignation::BL);
            break;
        case POINT_RIGHT:
            error_desig += checkDangerReading(directionDesignation::R);
            error_desig += checkDangerReading(directionDesignation::FR);
            error_desig += checkDangerReading(directionDesignation::BR);
            break;
        case FWD_LEFT:
            error_desig += checkDangerReading(directionDesignation::L);
            error_desig += checkDangerReading(directionDesignation::FL);
            error_desig += checkDangerReading(directionDesignation::F);
            break;
        case FWD_RIGHT:
            error_desig += checkDangerReading(directionDesignation::R);
            error_desig += checkDangerReading(directionDesignation::FR);
            error_desig += checkDangerReading(directionDesignation::F);
            break;
        case BWD_LEFT:
            error_desig += checkDangerReading(directionDesignation::L);
            error_desig += checkDangerReading(directionDesignation::BL);
            error_desig += checkDangerReading(directionDesignation::B);
            break;
        case BWD_RIGHT:
            error_desig += checkDangerReading(directionDesignation::R);
            error_desig += checkDangerReading(directionDesignation::BR);
            error_desig += checkDangerReading(directionDesignation::B);
            break;
    }
    return error_desig;
}
