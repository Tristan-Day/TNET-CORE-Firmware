#include <BLE.hpp>

#define DEBUG

namespace Bluetooth
{
    BLEServer *pServer;
    BLEAdvertising *pAdvertising;

    bool clientConnected;
}

void Bluetooth::ServerCallbacks::onConnect(BLEServer *pServer)
{
    clientConnected = true;
#ifdef DEBUG
    Serial.println("[INFO] BLE Client Connected");
#endif
}

void Bluetooth::ServerCallbacks::onDisconnect(BLEServer *pServer)
{
    clientConnected = false;
    pAdvertising->start();
#ifdef DEBUG
    Serial.println("[INFO] BLE Client Disconnected");
#endif
}

void Bluetooth::initialise(std::string deviceName)
{
    BLEDevice::init(deviceName);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new Bluetooth::ServerCallbacks());

    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(BLE_SERVICE_UUID);

#ifdef DEBUG
    Serial.println("[INFO] BLE Device Initialised");
#endif
}

void Bluetooth::startAdvertising()
{
    pAdvertising->start();
#ifdef DEBUG
    Serial.println("[INFO] BLE Advertising Started");
#endif
}
