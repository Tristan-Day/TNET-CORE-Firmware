#include <service/Environment.hpp>

EnvironmentService::EnvironmentService() : Service()
{
    if (BME688::get()->hasFailed())
    {
        setFailed(true);
        return;
    }

    temperatureCharacteristic = new BLECharacteristic(
        BLEUUID((uint16_t)0x2A6E),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

    humidityCharacteristic = new BLECharacteristic(
        BLEUUID((uint16_t)0x2A6F),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

    pressureCharacteristic = new BLECharacteristic(
        BLEUUID((uint16_t)0x2A6D),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

    altitudeCharacteristic = new BLECharacteristic(
        BLEUUID((uint16_t)0x2AB3),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

    service = Bluetooth::get()->pServer->createService(BLEUUID((uint16_t)0x181A));

    service->addCharacteristic(temperatureCharacteristic);
    temperatureCharacteristic->addDescriptor(new BLE2902());

    service->addCharacteristic(pressureCharacteristic);
    pressureCharacteristic->addDescriptor(new BLE2902());

    service->addCharacteristic(altitudeCharacteristic);
    altitudeCharacteristic->addDescriptor(new BLE2902());

    service->addCharacteristic(humidityCharacteristic);
    humidityCharacteristic->addDescriptor(new BLE2902());

    service->start();
};

void EnvironmentService::execute()
{
    BME688* sensor = BME688::get();

#ifdef DEBUG
    Serial.println("[INFO] Collecting Environmental Data");
#endif

    uint16_t temperature = sensor->getTemperature() * 100;
    temperatureCharacteristic->setValue((uint8_t*)&temperature, 2);
    temperatureCharacteristic->notify();

    uint32_t pressure = sensor->getPressure() * 10;
    pressureCharacteristic->setValue((uint8_t*)&pressure, 4);
    pressureCharacteristic->notify();

    uint16_t humidity = sensor->getHumidity() * 100;
    humidityCharacteristic->setValue((uint8_t*)&humidity, 2);
    humidityCharacteristic->notify();

    uint16_t altitude = sensor->getAltitude() * 10;
    altitudeCharacteristic->setValue((uint8_t*)&altitude, 2);
    altitudeCharacteristic->notify();
}

string EnvironmentService::getName()
{
    return "Environment Service";
}
