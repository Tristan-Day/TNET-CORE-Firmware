#include <service/Battery.hpp>

const string BatteryService::BATTERY_VOLTAGE_UUID =
    "2cd85adb-31d1-4fb9-b437-6288b780ca43";

BatteryService::BatteryService() : Service()
{
    chargeCharacteristic = new BLECharacteristic(
        BLEUUID((uint16_t)0x2A19),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

    voltageCharacteristic = new BLECharacteristic(
        BATTERY_VOLTAGE_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

    service = Bluetooth::get()->server->createService(BLEUUID((uint16_t)0x180F));

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
    uint8_t charge = MAX17048::get()->sensor->cellPercent();
    chargeCharacteristic->setValue((uint8_t*)&charge, 1);

    uint16_t voltage = MAX17048::get()->sensor->cellVoltage() * 10;
    voltageCharacteristic->setValue((uint8_t*)&voltage, 2);
}

string BatteryService::getName()
{
    return "Battery Service";
}
