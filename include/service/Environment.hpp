#pragma once

#include <hardware/Bluetooth.hpp>
#include <hardware/sensor/BME688.hpp>

#include <BLE2902.h>
#include <Preferences.h>

class EnvironmentService : public Service
{
  private:
    static constexpr float SEA_LEVEL_PRESSURE = 1013.25;

    BLEService* service = nullptr;

    BLECharacteristic* temperatureCharacteristic;
    BLECharacteristic* humidityCharacteristic;
    BLECharacteristic* pressureCharacteristic;
    BLECharacteristic* altitudeCharacteristic;

    void execute() override;

  public:
    EnvironmentService();

    string getName() override;
};