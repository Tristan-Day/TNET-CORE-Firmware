#pragma once

#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>

class Bluetooth : public BLEServerCallbacks
{
  private:
    static constexpr uint8_t TRANSMISSION_UNIT = 30;
    static constexpr esp_power_level_t TRANSMISSION_POWER = ESP_PWR_LVL_N24;

    static std::string NAME;

    static Bluetooth* instance;

    Bluetooth();

    void onConnect(BLEServer* server);

    void onDisconnect(BLEServer* server);

  public:
    EventGroupHandle_t bluetoothEventGroup;

    BLEServer* server = nullptr;
    BLEAdvertising* advertising = nullptr;

    static void init();

    static Bluetooth* get();

    Bluetooth(Bluetooth& other) = delete;

    void operator=(const Bluetooth&) = delete;
};