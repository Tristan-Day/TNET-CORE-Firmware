#pragma once

#include <Arduino.h>

#include <NimBLEDevice.h>
#include <NimBLEServer.h>

constexpr uint8_t TRANSMISSION_POWER = ESP_PWR_LVL_N0;
constexpr const char* DEVICE_NAME = "TNET - CORE (Series 2)";

class Bluetooth
{
  private:
    static Bluetooth* instance;

    Bluetooth();

    class Callbacks : public BLEServerCallbacks
    {
        void onConnect(BLEServer* server) override;

        void onDisconnect(BLEServer* server) override;
    };

  public:
    EventGroupHandle_t connectionEvent;

    BLEServer* server;
    BLEAdvertising* advertising;

    static Bluetooth* get();

    Bluetooth(Bluetooth& other) = delete;

    void operator=(const Bluetooth&) = delete;

    void enable();

    void disable();
};

class BluetoothService
{
  protected:
    BLEService* service;

  public:
    virtual void refresh();
};