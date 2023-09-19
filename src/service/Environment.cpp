#include <service/Environment.hpp>

EnvironmentService::EnvironmentService()
{
    if (BME688::get()->hasFailed())
    {
        return;
    }

    service = Bluetooth::get()->server->createService(BLEUUID((uint16_t)0x181A));

    TMP = service->createCharacteristic(
        BLEUUID((uint16_t)0x2A6E), NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

    PRE = service->createCharacteristic(
        BLEUUID((uint16_t)0x2A6D), NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

    HUM = service->createCharacteristic(
        BLEUUID((uint16_t)0x2A6F), NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

    ALT = service->createCharacteristic(
        BLEUUID((uint16_t)0x2AB3), NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

    CO2 = service->createCharacteristic(
        BLEUUID((uint16_t)0x2B8C), NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

    VOC = service->createCharacteristic(
        BLEUUID((uint16_t)0x2BE7), NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

    service->start();
}

void EnvironmentService::referesh()
{
    BME688* sensor = BME688::get();

    // Atmospheric Indicators

    uint16_t temperature = sensor->TMP->get() * 100;
    TMP->setValue((uint8_t*)&temperature, 2);
    TMP->notify();

    uint32_t pressure = sensor->PRE->get() * 10;
    PRE->setValue((uint8_t*)&pressure, 4);
    PRE->notify();

    uint16_t humidity = sensor->HUM->get() * 100;
    HUM->setValue((uint8_t*)&humidity, 2);
    HUM->notify();

    uint16_t altitude = sensor->ALT->get() * 10;
    ALT->setValue((uint8_t*)&altitude, 2);
    ALT->notify();

    // Air Quality Indicators

    uint16_t carbonDioxide = sensor->CO2->get();
    CO2->setValue((uint8_t*)&carbonDioxide);
    CO2->notify();

    uint16_t volitileCompounds = sensor->VOC->get();
    VOC->setValue((uint8_t*)&volitileCompounds);
    VOC->notify();

#ifdef DEBUG
    Serial.println("[INFO] Environmental Service Attributes Updated");
#endif
}