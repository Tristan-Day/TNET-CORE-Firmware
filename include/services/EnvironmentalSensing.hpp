#pragma once

#include <BLE2902.h>
#include <BLEUtils.h>
#include <Preferences.h>

#include <SystemTask.hpp>
#include <hardware/sensor/BME280.hpp>

class EnvironmentService : public SystemTask
{
private:
    static const uint16_t TASK_STACK_DEPTH = 2000;
    static constexpr float SEA_LEVEL_PRESSURE = 1013.25;

    static EnvironmentService *pInstance;

    BLEService *pService;

    BLECharacteristic temperatureCharacteristic;

    BLECharacteristic pressureCharacteristic;

    BLECharacteristic altitudeCharacteristic;

    BLECharacteristic humidityCharacteristic;

    EnvironmentService()
        : temperatureCharacteristic(BLEUUID((uint16_t)0x2A6E),
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY),

          pressureCharacteristic(BLEUUID((uint16_t)0x2A6D),
              BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY),

          altitudeCharacteristic(BLEUUID((uint16_t)0x2AB3),
              BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY),

          humidityCharacteristic(BLEUUID((uint16_t)0x2A6F),
              BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY) {};

    static void task(void *);

public:
    EnvironmentService(const EnvironmentService &obj) = delete;

    static EnvironmentService *get();

    void init(BLEServer *pServer);
};