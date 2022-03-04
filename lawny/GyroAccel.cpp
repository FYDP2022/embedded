#include "GyroAccel.hpp"
#include <Wire.h>

#define DEBUG

unsigned long timer;

GyroAccel::GyroAccel() : mpu(Wire) {
    return;
}

GyroAccel::~GyroAccel() {
    return;
}

bool GyroAccel::init() {
    byte status = mpu.begin(GYRO_500, ACEL_2G);
    if(status != 0) {
        return false;
    }
    // Calibrate Sensor 
    mpu.calcOffsets();
    timer = millis();
    return true;
}

void GyroAccel::update_state() {
    mpu.update();
}

void GyroAccel::debug_print() {
    if(millis() - timer > 1000){ // print data max every second
        Serial.print(F("TEMPERATURE: "));Serial.println(mpu.getTemp());
        Serial.print(F("ACCELERO  X: "));Serial.print(mpu.getAccX());
        Serial.print("\tY: ");Serial.print(mpu.getAccY());
        Serial.print("\tZ: ");Serial.println(mpu.getAccZ());
    
        Serial.print(F("GYRO      X: "));Serial.print(mpu.getGyroX());
        Serial.print("\tY: ");Serial.print(mpu.getGyroY());
        Serial.print("\tZ: ");Serial.println(mpu.getGyroZ());
    
        Serial.print(F("ACC ANGLE X: "));Serial.print(mpu.getAccAngleX());
        Serial.print("\tY: ");Serial.println(mpu.getAccAngleY());
        
        Serial.print(F("ANGLE     X: "));Serial.print(mpu.getAngleX());
        Serial.print("\tY: ");Serial.print(mpu.getAngleY());
        Serial.print("\tZ: ");Serial.println(mpu.getAngleZ());
        Serial.println(F("=====================================================\n"));
        timer = millis();
    }
}

bool GyroAccel::isFlat(int threshold) {
    float X = mpu.getAngleX();
    float Y = mpu.getAngleY();
    if(abs(X) < 10.0 && abs(Y) < 10.0) {
        return true;
    }
    #ifdef DEBUG
    Serial.println("LAWNY DETECTED NOT FLAT!");
    #endif
    return false;
}