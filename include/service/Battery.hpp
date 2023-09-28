#pragma once

#include <hardware/Bluetooth.hpp>
#include <hardware/sensor/MAX17048.hpp>

class BatteryService
{
  private:
    static constexpr const char* BATTERY_VOLTAGE_UUID =
        "2cd85adb-31d1-4fb9-b437-6288b780ca43";

    BLEService* service;

    // @brief Battery Charge Percentage
    BLECharacteristic* CHG;

    // @brief Battery Voltage in mV
    BLECharacteristic* VLT;

  public:
    BatteryService();

    void refresh();
};