#include <service/Battery.hpp>

BatteryService::BatteryService()
{
    service = Bluetooth::get()->server->createService(BLEUUID((uint16_t)0x180F));

    CHG = service->createCharacteristic(
        BLEUUID((uint16_t)0x2A19), NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

    VLT = service->createCharacteristic(
        BLEUUID(BATTERY_VOLTAGE_UUID), NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

    service->start();
}

BatteryService::~BatteryService()
{
    service->removeCharacteristic(CHG, true);
    service->removeCharacteristic(VLT, true);

    Bluetooth::get()->server->removeService(service, true);
}

void BatteryService::refresh()
{
    uint8_t charge = MAX17048::get()->CHG->get();
    CHG->setValue((uint8_t*)&charge, 1);
    CHG->notify();

    uint16_t voltage = MAX17048::get()->VLT->get() * 10;
    VLT->setValue((uint8_t*)&voltage, 2);
    VLT->notify();

#ifdef DEBUG
    Serial.println("[INFO] Battery Service Attributes Updated");
#endif
}