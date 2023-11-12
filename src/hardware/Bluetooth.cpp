#include <hardware/Bluetooth.hpp>

Bluetooth* Bluetooth::instance = nullptr;

Bluetooth::Bluetooth()
{
    connectionEvent = xEventGroupCreate();
}

void Bluetooth::Callbacks::onConnect(BLEServer* pServer)
{
    Bluetooth* self = Bluetooth::get();

    self->advertising->stop();

    // Clear Disconnect Flag
    static const EventBits_t xBitsToClear = (1 << 1);
    xEventGroupClearBits(self->connectionEvent, xBitsToClear);

    // Set Connected Flag
    static const EventBits_t xBitsToSet = (1 << 0);
    xEventGroupSetBits(self->connectionEvent, xBitsToSet);

#ifdef DEBUG
    Serial.println("[INFO] BLE Client Connected");
#endif
}

void Bluetooth::Callbacks::onDisconnect(BLEServer* pServer)
{
    Bluetooth* self = Bluetooth::get();

    // Clear Connected Flag
    static const EventBits_t xBitsToClear = (1 << 0);
    xEventGroupClearBits(self->connectionEvent, xBitsToClear);

    // Set Disconnect Flag
    static const EventBits_t xBitsToSet = (1 << 1);
    xEventGroupSetBits(self->connectionEvent, xBitsToSet);

    self->advertising->start();

#ifdef DEBUG
    Serial.println("[INFO] BLE Client Disconnected");
#endif
}

Bluetooth* Bluetooth::get()
{
    if (instance == nullptr)
    {
        instance = new Bluetooth();
    }
    return instance;
}

void Bluetooth::enable()
{
    BLEDevice::init(DEVICE_NAME);
    BLEDevice::setPower((esp_power_level_t)TRANSMISSION_POWER);

    server = BLEDevice::createServer();
    server->setCallbacks(new Callbacks());

    advertising = BLEDevice::getAdvertising();
    advertising->setScanResponse(false);

#ifdef DEBUG
    Serial.println("[INFO] Bluetooth Enabled");
#endif
}

void Bluetooth::disable()
{
    BLEDevice::deinit(true);

#ifdef DEBUG
    Serial.println("[INFO] Bluetooth Disabled");
#endif
}
