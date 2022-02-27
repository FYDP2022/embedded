#ifndef GYROACCEL
#define GYROACCEL

#include "Wire.h"
#include <MPU6050_light.h>

#define GYRO_250 0
#define GYRO_500 1
#define GYRO_1000 2
#define GYRO_2000 3

#define ACEL_2G 0
#define ACEL_4G 1
#define ACEL_8G 2
#define ACEL_16G 3

class GyroAccel {
    public:
        GyroAccel();
        ~GyroAccel();
        bool init();

        void update_state();

        void debug_print();
        // float getGyroX();
        // float getGyroY();
        // float getGyroX();

        // float getAccelX();
        // float getAccelY();
        // float getAccelZ();

        // float getAngleX();
        // float getAngleY();

        bool isFlat(int threshold=10);

    private:
        MPU6050 mpu;
};

extern GyroAccel gyroaccel;

#endif