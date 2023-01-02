#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define BLE_SERVICE_UUID "fc4582b1-6203-4eae-bca6-ba6f8cc38061"

namespace Bluetooth
{

    extern BLEServer *pServer;
    extern BLEAdvertising *pAdvertising;

    extern bool clientConnected;

    class ServerCallbacks : public BLEServerCallbacks
    {
        void onConnect(BLEServer *pServer);

        void onDisconnect(BLEServer *pServer);
    };

    void initialise(std::string deviceName);

    void startAdvertising();

}