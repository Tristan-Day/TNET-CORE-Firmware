#include <hardware/Bluetooth.hpp>

#define DEBUG

Bluetooth *Bluetooth::pInstance;

void Bluetooth::onConnect(BLEServer *pServer)
{
    // Clear Disconnect Flag
    static const EventBits_t xBitsToClear = (1 << 1);
    xEventGroupClearBits(serverEventGroup, xBitsToClear);

    // Set Connected Flag
    static const EventBits_t xBitsToSet = (1 << 0);
    xEventGroupSetBits(serverEventGroup, xBitsToSet);

#ifdef DEBUG
    Serial.println("[INFO] BLE Client Connected");
#endif
}

void Bluetooth::onDisconnect(BLEServer *pServer)
{
    // Clear Connected Flag
    static const EventBits_t xBitsToClear = (1 << 0);
    xEventGroupClearBits(serverEventGroup, xBitsToClear);

    // Set Disconnect Flag
    static const EventBits_t xBitsToSet = (1 << 1);
    xEventGroupSetBits(serverEventGroup, xBitsToSet);

    pAdvertising->start();

#ifdef DEBUG
    Serial.println("[INFO] BLE Client Disconnected");
#endif
}

Bluetooth *Bluetooth::get()
{
    if (pInstance == NULL)
    {
        pInstance = new Bluetooth();
    }
    return pInstance;
}

void Bluetooth::init(std::string deviceName)
{
    serverEventGroup = xEventGroupCreate();

    BLEDevice::init(deviceName);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(get());

    pAdvertising = BLEDevice::getAdvertising();

#ifdef DEBUG
    Serial.println("[INFO] Bluetooth Device Initialised");
#endif
}

void Bluetooth::startAdvertising()
{
    pAdvertising->start();

#ifdef DEBUG
    Serial.println("[INFO] BLE Advertising Started");
#endif
}