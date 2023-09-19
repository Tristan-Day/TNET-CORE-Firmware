#pragma once

#include <Arduino.h>

#include <NimBLEDevice.h>
#include <NimBLEServer.h>

class Bluetooth : public NimBLEServerCallbacks
{
  private:
    static constexpr int8_t TRANSMISSION_POWER = ESP_PWR_LVL_P6;
    static constexpr const char* DEVICE_NAME = "TNET - CORE (Series 2)";

    static Bluetooth* instance;

    Bluetooth();

    void onConnect(BLEServer* server);

    void onDisconnect(BLEServer* server);

  public:
    EventGroupHandle_t connectionEvent;

    NimBLEServer* server;
    NimBLEAdvertising* advertising;

    static void init();

    static Bluetooth* get();

    Bluetooth(Bluetooth& other) = delete;

    void operator=(const Bluetooth&) = delete;
};