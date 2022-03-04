#include "Temperature.hpp"

OneWire onewire(TEMP_DATA_PIN);
DallasTemperature sensors(&onewire);

struct DeviceAddressStruct {
    DeviceAddress raw;
    DeviceAddressStruct(const DeviceAddress a) {
        memcpy(raw, a, sizeof(DeviceAddress));
    }
};

const DeviceAddress addr0 = {0x28, 0x0E, 0x26, 0x96, 0xF0, 0x01, 0x3C, 0xD5};
const DeviceAddress addr1 = {0x28, 0xDB, 0xAE, 0x96, 0xF0, 0x01, 0x3C, 0x0F};
const DeviceAddress addr2 = {0x28, 0x73, 0x3F, 0x96, 0xF0, 0x01, 0x3C, 0x88};
const DeviceAddress addr3 = {0x28, 0x7B, 0xFC, 0x96, 0xF0, 0x01, 0x3C, 0x45};
const DeviceAddress addr4 = {0x28, 0x5D, 0x5F, 0x96, 0xF0, 0x01, 0x3C, 0x3C};

DeviceAddressStruct sensor_addr[SENSOR_COUNT] = {addr0, addr1, addr2, addr3, addr4};
//280E2696F0013CD5
//28DBAE96F0013C0F
//28733F96F0013C88
//287BFC96F0013C45
//285D5F96F0013C3C

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

TemperatureSensor::TemperatureSensor() {
    foundDeviceCount = 0;
    memset(temperatures, 0, SENSOR_COUNT * sizeof(float));
}

TemperatureSensor::~TemperatureSensor() {

}

bool TemperatureSensor::init() {
    sensors.begin();
    foundDeviceCount = sensors.getDeviceCount();
    if(foundDeviceCount != SENSOR_COUNT) {
        return false;
    }
    sensors.setResolution(TEMP_RESOLUTION);
    sensors.setWaitForConversion(false);
    sensors.requestTemperatures();
    lastTempRequest = millis();
    delayInMillis = 750 / (1 << (12 - TEMP_RESOLUTION));
    return true;
}

void TemperatureSensor::update_state() {
    if (millis() - lastTempRequest >= delayInMillis) {
        for(int i =0; i < SENSOR_COUNT; i++) {
            temperatures[i] = sensors.getTempC(sensor_addr[i].raw);
        }
        sensors.requestTemperatures();
        lastTempRequest = millis();  
    }
    // TODO: Create proper interval to send sensor data, maybe another module?
}

float TemperatureSensor::getTemperature(int index) {
    if(index >= 0 && index < SENSOR_COUNT)
        return temperatures[index];
    return 0.0;
}