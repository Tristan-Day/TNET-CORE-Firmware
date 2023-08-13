#pragma once

#include <hardware/Bluetooth.hpp>
#include <service/Service.hpp>

#include <BLE2902.h>
#include <Adafruit_MAX1704X.h>

class BatteryService : public Service
{
  private:
    static const string BATTERY_VOLTAGE_UUID;

    BLEService* service;

    BLECharacteristic* chargeCharacteristic;
    BLECharacteristic* voltageCharacteristic;

    void execute() override;

  public:
    Adafruit_MAX17048* battery = nullptr;

    BatteryService();

    string getName() override;
};