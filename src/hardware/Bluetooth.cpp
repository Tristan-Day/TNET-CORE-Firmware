#include <hardware/Bluetooth.hpp>

Bluetooth* Bluetooth::instance = nullptr;

Bluetooth::Bluetooth()
{
    connectionEvent = xEventGroupCreate();

    BLEDevice::init(DEVICE_NAME);
    BLEDevice::setPower((esp_power_level_t)TRANSMISSION_POWER);

    server = BLEDevice::createServer();
    server->setCallbacks(this);
    
    advertising = BLEDevice::getAdvertising();
    advertising->setScanResponse(false);

#ifdef DEBUG
    Serial.println("[INFO] Bluetooth Device Initialised");
#endif
}

void Bluetooth::onConnect(BLEServer* pServer)
{
    advertising->stop();

    // Clear Disconnect Flag
    static const EventBits_t xBitsToClear = (1 << 1);
    xEventGroupClearBits(connectionEvent, xBitsToClear);

    // Set Connected Flag
    static const EventBits_t xBitsToSet = (1 << 0);
    xEventGroupSetBits(connectionEvent, xBitsToSet);

#ifdef DEBUG
    Serial.println("[INFO] BLE Client Connected");
#endif
}

void Bluetooth::onDisconnect(BLEServer* pServer)
{
    // Clear Connected Flag
    static const EventBits_t xBitsToClear = (1 << 0);
    xEventGroupClearBits(connectionEvent, xBitsToClear);

    // Set Disconnect Flag
    static const EventBits_t xBitsToSet = (1 << 1);
    xEventGroupSetBits(connectionEvent, xBitsToSet);

    advertising->start();

#ifdef DEBUG
    Serial.println("[INFO] BLE Client Disconnected");
#endif
}

void Bluetooth::init()
{
    if (instance == nullptr)
    {
        instance = new Bluetooth();
    }
}

Bluetooth* Bluetooth::get()
{
    if (instance == nullptr)
    {
        init();
    }
    return instance;
}
