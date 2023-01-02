#include <Arduino.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <Devices.hpp>

#define HEART_RATE_SERVICE_TASK_STACK_DEPTH 2000

namespace HeartRateService
{
    extern BLEService *pService;
    extern TaskHandle_t taskHandle;

    extern BLECharacteristic heartRateCharacteristic;

    extern BLECharacteristic sensorLocationCharacteristic;
    extern BLEDescriptor sensorLocationDescriptor;

    extern BLECharacteristic sensorTemperatureCharacteristic;
    extern BLEDescriptor sensorTemperatureDescriptor;

    extern const uint8_t sensorLocation;

    void createService(BLEServer *pServer, BLEAdvertising *pAdvertising);

    void serviceTask(void*);
}