#pragma once

#include <hardware/Bluetooth.hpp>
#include <hardware/sensor/BME688.hpp>

class EnvironmentService : public BluetoothService
{
  private:
    // @brief Temperature in Degrees Celsius
    BLECharacteristic* TMP;

    // @brief Atmospheric Humidity
    BLECharacteristic* HUM;

    // @brief Atmospheric Pressure
    BLECharacteristic* PRE;

    // @brief Altitude in M
    BLECharacteristic* ALT;

    // @brief Carbon Dioxide Levels in PPM
    BLECharacteristic* CO2;

    // @brief Volitile Organic Compounds in PPM
    BLECharacteristic* VOC;

  public:
    EnvironmentService();

    ~EnvironmentService();

    void refresh() override;
};
