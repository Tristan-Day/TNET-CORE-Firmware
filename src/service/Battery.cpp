#include <service/Battery.hpp>

const string BatteryService::BATTERY_VOLTAGE_UUID =
    "2cd85adb-31d1-4fb9-b437-6288b780ca43";

BatteryService::BatteryService() : Service()
{
    battery = new Adafruit_MAX17048();

    if (not battery->begin())
    {
        setFailed(true);
        return;
    }

    chargeCharacteristic = new BLECharacteristic(
        BLEUUID((uint16_t)0x2A19),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

    voltageCharacteristic = new BLECharacteristic(
        BATTERY_VOLTAGE_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

    service = Bluetooth::get()->pServer->createService(BLEUUID((uint16_t)0x180F));

    service->addCharacteristic(chargeCharacteristic);
    chargeCharacteristic->addDescriptor(new BLE2902);

    service->addCharacteristic(voltageCharacteristic);
    voltageCharacteristic->addDescriptor(new BLE2902);

    service->start();
}

void BatteryService::execute()
{
#ifdef DEBUG
    Serial.println("[INFO] Collecting Battery Data");
#endif

    uint8_t charge = battery->cellPercent();
    chargeCharacteristic->setValue((uint8_t*)&charge, 1);

    uint16_t voltage = battery->cellVoltage() * 10;
    voltageCharacteristic->setValue((uint8_t*)&voltage, 2);
}

string BatteryService::getName()
{
    return "Battery Service";
}
