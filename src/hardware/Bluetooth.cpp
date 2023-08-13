#include <hardware/Bluetooth.hpp>

Bluetooth* Bluetooth::instance = nullptr;

Bluetooth::Bluetooth()
{
    bluetoothEventGroup = xEventGroupCreate();

    BLEDevice::init(DEVICE_NAME);
    BLEDevice::setPower(TRANSMISSION_POWER);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(this);

    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->start();

#ifdef DEBUG
    Serial.println("[INFO] Bluetooth Device Initialised");
#endif
}

void Bluetooth::onConnect(BLEServer* pServer)
{
    // Clear Disconnect Flag
    static const EventBits_t xBitsToClear = (1 << 1);
    xEventGroupClearBits(bluetoothEventGroup, xBitsToClear);

    // Set Connected Flag
    static const EventBits_t xBitsToSet = (1 << 0);
    xEventGroupSetBits(bluetoothEventGroup, xBitsToSet);

#ifdef DEBUG
    Serial.println("[INFO] BLE Client Connected");
#endif
}

void Bluetooth::onDisconnect(BLEServer* pServer)
{
    // Clear Connected Flag
    static const EventBits_t xBitsToClear = (1 << 0);
    xEventGroupClearBits(bluetoothEventGroup, xBitsToClear);

    // Set Disconnect Flag
    static const EventBits_t xBitsToSet = (1 << 1);
    xEventGroupSetBits(bluetoothEventGroup, xBitsToSet);

    pAdvertising->start();

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
