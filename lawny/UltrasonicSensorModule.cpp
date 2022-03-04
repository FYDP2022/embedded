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

int UltrasonicSensorModule::update_state() {
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
        CommunicationInterface::writeErrorToSerial(moduleName, "state_error", sensor_name + "_detected_close");
        return -1;
    }
    Serial.println(convertDirectionToName(direction) + " " + distance);
    return 0;
}

int UltrasonicSensorModule::isSafeUltraSonic(DRIVE_CMD_ENUM c) {
    int error_desig = 0;
    switch(c) {
        case DRIVE_CMD_ENUM::FORWARD:
            error_desig += checkDangerReading(directionDesignation::F);
            break;
        case DRIVE_CMD_ENUM::REVERSE:
            error_desig += checkDangerReading(directionDesignation::B);
            break;
        case DRIVE_CMD_ENUM::STOP:
            break;
        case DRIVE_CMD_ENUM::POINT_LEFT:
            error_desig += checkDangerReading(directionDesignation::L);
            error_desig += checkDangerReading(directionDesignation::FL);
            error_desig += checkDangerReading(directionDesignation::BL);
            break;
        case DRIVE_CMD_ENUM::POINT_RIGHT:
            error_desig += checkDangerReading(directionDesignation::R);
            error_desig += checkDangerReading(directionDesignation::FR);
            error_desig += checkDangerReading(directionDesignation::BR);
            break;
        case DRIVE_CMD_ENUM::FWD_LEFT:
            error_desig += checkDangerReading(directionDesignation::L);
            error_desig += checkDangerReading(directionDesignation::FL);
            error_desig += checkDangerReading(directionDesignation::F);
            break;
        case DRIVE_CMD_ENUM::FWD_RIGHT:
            error_desig += checkDangerReading(directionDesignation::R);
            error_desig += checkDangerReading(directionDesignation::FR);
            error_desig += checkDangerReading(directionDesignation::F);
            break;
        case DRIVE_CMD_ENUM::BWD_LEFT:
            error_desig += checkDangerReading(directionDesignation::L);
            error_desig += checkDangerReading(directionDesignation::BL);
            error_desig += checkDangerReading(directionDesignation::B);
            break;
        case DRIVE_CMD_ENUM::BWD_RIGHT:
            error_desig += checkDangerReading(directionDesignation::R);
            error_desig += checkDangerReading(directionDesignation::BR);
            error_desig += checkDangerReading(directionDesignation::B);
            break;
    }
    return error_desig;
}
