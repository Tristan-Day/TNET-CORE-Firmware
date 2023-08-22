#include <hardware/Bluetooth.hpp>

std::string Bluetooth::NAME = "TNET CORE - (Series 2)";

Bluetooth* Bluetooth::instance = nullptr;

Bluetooth::Bluetooth()
{
    bluetoothEventGroup = xEventGroupCreate();

    BLEDevice::init(NAME);

    BLEDevice::setPower(TRANSMISSION_POWER);
    BLEDevice::setMTU(TRANSMISSION_UNIT);

    server = BLEDevice::createServer();

    server->setCallbacks(this);

    esp_bd_addr_t ADDRESS = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
    server->updateConnParams(ADDRESS, 12, 12, 12, 400);

    advertising = BLEDevice::getAdvertising();
    advertising->start();

#ifdef DEBUG
    Serial.println("[INFO] Bluetooth Device Initialised");
#endif
}

void Bluetooth::onConnect(BLEServer* pServer)
{
    advertising->stop();

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
