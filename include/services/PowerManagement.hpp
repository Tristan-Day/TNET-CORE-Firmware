#pragma once

#include <BLE2902.h>
#include <Preferences.h>

#include <hardware/Battery.hpp>
#include <hardware/Bluetooth.hpp>

#include <hardware/sensor/BME280.hpp>
#include <hardware/sensor/GY8511.hpp>
#include <hardware/sensor/MAX30105.hpp>

namespace PowerProfile
{
    static void active();

    static void optimised();

    static void powerSaving();

    static void ultaPowerSaving();

    enum
    {
        ACTIVE,
        OPTIMISED,
        POWER_SAVING,
        ULTRA_POWER_SAVING
    };
};

class PowerManagementService : public BLECharacteristicCallbacks
{
private:
    static const std::string POWER_PROFILE_UUID;

    static const uint16_t TASK_STACK_DEPTH = 2000;

    static PowerManagementService *pInstance;

    Preferences preferences;
    BLEService *pService;

    BLECharacteristic batteryCharageCharacteristic;

    BLECharacteristic powerProfileCharacteristic;
    BLEDescriptor powerProfileDescriptor;

    PowerManagementService()
        : batteryCharageCharacteristic(
            BLEUUID((uint16_t)0x2A19), BLECharacteristic::PROPERTY_READ),

          powerProfileCharacteristic(POWER_PROFILE_UUID,
              BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE),
          powerProfileDescriptor(BLEUUID((uint16_t)0x2901)) {};

    void onWrite(BLECharacteristic *pCharacteristic);

    static void bluetoothEventHandler(void *);

    static void batteryMonitorTask(void *);

public:
    PowerManagementService(const PowerManagementService &obj) = delete;

    static PowerManagementService *get();

    void init(BLEServer *pServer);

    void setProfile(uint8_t profile);
};