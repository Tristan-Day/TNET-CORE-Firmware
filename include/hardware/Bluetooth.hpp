#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

class Bluetooth : public BLEServerCallbacks
{
private:
    static Bluetooth *pInstance;

    Bluetooth() { }

    void onConnect(BLEServer *pServer);

    void onDisconnect(BLEServer *pServer);

public:
    EventGroupHandle_t serverEventGroup;

    BLEServer *pServer;
    BLEAdvertising *pAdvertising;

    Bluetooth(const Bluetooth &obj) = delete;

    static Bluetooth *get();

    void init(std::string deviceName);

    void startAdvertising();
};