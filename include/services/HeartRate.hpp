#pragma once

#include <BLE2902.h>
#include <BLEUtils.h>

#include <SystemTask.hpp>
#include <hardware/sensor/MAX30105.hpp>

class HeartRateSensing : public SystemTask
{
private:
    static const uint16_t TASK_STACK_DEPTH = 2000;
    static const uint8_t SENSOR_LOCATION = 1;

    static HeartRateSensing *pInstance;

    BLEService *pService;

    BLECharacteristic heartRateCharacteristic;

    BLECharacteristic sensorLocationCharacteristic;
    BLEDescriptor sensorLocationDescriptor;

    BLECharacteristic sensorTemperatureCharacteristic;
    BLEDescriptor sensorTemperatureDescriptor;

    HeartRateSensing()
        : heartRateCharacteristic(BLEUUID((uint16_t)0x2A37),
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY),

          sensorLocationCharacteristic(
              BLEUUID((uint16_t)0x2A38), BLECharacteristic::PROPERTY_READ),
          sensorLocationDescriptor(BLEUUID((uint16_t)0x2901)),

          sensorTemperatureCharacteristic(BLEUUID((uint16_t)0x2A6E),
              BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY),
          sensorTemperatureDescriptor(BLEUUID((uint16_t)0x2901)) {};

    static void task(void *);

public:
    HeartRateSensing(const HeartRateSensing &obj) = delete;

    static HeartRateSensing *get();

    void init(BLEServer *pServer, BLEAdvertising *pAdvertising);
};