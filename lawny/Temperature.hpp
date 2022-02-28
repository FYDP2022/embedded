#ifndef TEMPERATURE_MODULE
#define TEMPERATURE_MODULE
#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMP_DATA_PIN A15
#define TEMP_RESOLUTION 9
#define SENSOR_COUNT 5

class TemperatureSensor {
    public:
        TemperatureSensor();
        ~TemperatureSensor();
        void init();
        
        void update_state();

        float getTemperature(int index);
    private:
        int foundDeviceCount;
        float temperatures[SENSOR_COUNT];
        unsigned long lastTempRequest = 0;
        int delayInMillis = 0;
};


extern TemperatureSensor temp_sensor;

#endif