#pragma once

#include <hardware/Bluetooth.hpp>
#include <hardware/sensor/MAX17048.hpp>

#include <service/Service.hpp>

#include <BLE2902.h>

class BatteryService : public Service
{
  private:
    static const string BATTERY_VOLTAGE_UUID;

    BLEService* service;

    BLECharacteristic* chargeCharacteristic;
    BLECharacteristic* voltageCharacteristic;

    void execute() override;

  public:
    BatteryService();

    string getName() override;
};