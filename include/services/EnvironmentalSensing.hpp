#include <Arduino.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <Devices.hpp>

#define SEA_LEVEL_PRESSURE 1013.25

#define ENVIRONMENTAL_SENSING_TASK_STACK_DEPTH 2000

namespace EnvironmentalSensing
{
    extern BLEService *pService;
    extern TaskHandle_t taskHandle;

    extern BLECharacteristic temperatureCharacteristic;

    extern BLECharacteristic pressureCharacteristic;

    extern BLECharacteristic altitudeCharacteristic;

    extern BLECharacteristic humidityCharacteristic;

    void createService(BLEServer *pServer);

    void serviceTask(void*);
}