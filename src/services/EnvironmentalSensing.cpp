#include <services/EnvironmentalSensing.hpp>

#define DEBUG

namespace EnvironmentalSensing
{
    BLEService *pService;
    TaskHandle_t taskHandle;

    BLECharacteristic temperatureCharacteristic(
        BLEUUID((uint16_t)0x2A6E),
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY);

    BLECharacteristic pressureCharacteristic(
        BLEUUID((uint16_t)0x2A6D),
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY);

    BLECharacteristic altitudeCharacteristic(
        BLEUUID((uint16_t)0x2AB3),
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY);

    BLECharacteristic humidityCharacteristic(
        BLEUUID((uint16_t)0x2A6F),
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY);
}

void EnvironmentalSensing::createService(BLEServer *pServer)
{
    if (not BME280::connectionState){return;}

    pService = pServer->createService(BLEUUID((uint16_t)0x181A));

    pService->addCharacteristic(&temperatureCharacteristic);
    temperatureCharacteristic.addDescriptor(new BLE2902());

    pService->addCharacteristic(&pressureCharacteristic);
    pressureCharacteristic.addDescriptor(new BLE2902());

    pService->addCharacteristic(&altitudeCharacteristic);
    altitudeCharacteristic.addDescriptor(new BLE2902());

    pService->addCharacteristic(&humidityCharacteristic);
    humidityCharacteristic.addDescriptor(new BLE2902());

    pService->start();

    xTaskCreate(serviceTask,
                "Environmental Sensing Service",
                ENVIRONMENTAL_SENSING_TASK_STACK_DEPTH,
                NULL,
                tskIDLE_PRIORITY,
                &taskHandle);

    vTaskSuspend(taskHandle);
}

void EnvironmentalSensing::serviceTask(void*)
{
    while (true)
    {

#ifdef DEBUG
        Serial.println("[INFO] Reading Environmental Data");
#endif

        bool readSuccess = BME280::device.takeForcedMeasurement();
        if (not readSuccess) {Serial.println("[ERROR] BME280 Force Measurement"); return;}

        delay(1000);

        uint16_t temperature = BME280::device.readTemperature() * 100;
        temperatureCharacteristic.setValue((uint8_t *)&temperature, 2);
        temperatureCharacteristic.notify();

        uint32_t pressure = round(BME280::device.readPressure() * 10);
        pressureCharacteristic.setValue((uint8_t *)&pressure, 4);
        pressureCharacteristic.notify();

        uint16_t altitude = round(BME280::device.readAltitude(SEA_LEVEL_PRESSURE) * 10);
        altitudeCharacteristic.setValue((uint8_t *)&altitude, 2);
        altitudeCharacteristic.notify();

        uint16_t humidity = BME280::device.readHumidity() * 100;
        humidityCharacteristic.setValue((uint8_t *)&humidity, 2);
        humidityCharacteristic.notify();

        delay(BME280::sampleInterval);
    }
}