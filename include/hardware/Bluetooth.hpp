#pragma once

#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>

const std::string DEVICE_NAME = "TNET-CORE (Series 2)";
constexpr esp_power_level_t TRANSMISSION_POWER = ESP_PWR_LVL_N0;

class Bluetooth : public BLEServerCallbacks
{
  private:
    static Bluetooth* instance;

    Bluetooth();

    void onConnect(BLEServer* pServer);

    void onDisconnect(BLEServer* pServer);

  public:
    EventGroupHandle_t bluetoothEventGroup;

    BLEServer* pServer = nullptr;
    BLEAdvertising* pAdvertising = nullptr;

    static void init();

    static Bluetooth* get();

    Bluetooth(Bluetooth& other) = delete;

    void operator=(const Bluetooth&) = delete;
};