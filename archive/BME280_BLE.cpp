#include <BME280_BLE.hpp>

namespace BME280_BLE
{

    Adafruit_BME280 BME280; 

    BLEService *pService;
    TaskHandle_t taskHandle;

    BLECharacteristic temperatureCharacteristic(
        BLEUUID((uint16_t)0x2A6E),
        BLECharacteristic::PROPERTY_READ);
    BLEDescriptor temperatureDescriptor(BLEUUID((uint16_t)0x2901));

    BLECharacteristic pressureCharacteristic(
        BLEUUID((uint16_t)0x2A6D),
        BLECharacteristic::PROPERTY_READ);
    BLEDescriptor pressureDescriptor(BLEUUID((uint16_t)0x2901));

    BLECharacteristic altitudeCharacteristic(
        BLEUUID((uint16_t)0x2AB3),
        BLECharacteristic::PROPERTY_READ);
    BLEDescriptor altitudeDescriptor(BLEUUID((uint16_t)0x2901));

    BLECharacteristic humidityCharacteristic(
        BLEUUID((uint16_t)0x2A6F),
        BLECharacteristic::PROPERTY_READ);
    BLEDescriptor humidityDescriptor(BLEUUID((uint16_t)0x2901));

    bool connectionState;
    uint16_t sampleInterval;
}

void BME280_BLE::serviceTask(void *pParameters)
{
    while (true)
    {
        BME280.takeForcedMeasurement();

        uint16_t temperature = BME280.readTemperature() * 100;
        temperatureCharacteristic.setValue((uint8_t *)&temperature, 2);

        uint32_t pressure = round(BME280.readPressure() * 10);
        pressureCharacteristic.setValue((uint8_t *)&pressure, 4);

        uint16_t altitude = round(BME280.readAltitude(BME280_SEA_LEVEL_PRESSURE) * 10);
        altitudeCharacteristic.setValue((uint8_t *)&altitude, 2);

        uint16_t humidity = BME280.readHumidity() * 100;
        humidityCharacteristic.setValue((uint8_t *)&humidity, 2);

        delay(BME280_BLE::sampleInterval);
    }
}

void BME280_BLE::createService(BLEServer *pServer, uint16_t sampleInterval)
{
    BME280_BLE::sampleInterval = sampleInterval;
    connectionState = BME280_BLE::BME280.begin(_BME280_ADDRESS);
    BME280.setSampling(Adafruit_BME280::MODE_FORCED);

#ifdef BME280_BLE_VERBOSE
    if (not connectionState)
    {
        Serial.println("[INFO] BME280 not found");
    }
#endif

    pService = pServer->createService(BLEUUID((uint16_t)0x181A));

    pService->addCharacteristic(&temperatureCharacteristic);
    temperatureDescriptor.setValue("BME280 Temperature");
    temperatureCharacteristic.addDescriptor(&temperatureDescriptor);

    pService->addCharacteristic(&pressureCharacteristic);
    pressureDescriptor.setValue("BME280 Pressure");
    pressureCharacteristic.addDescriptor(&pressureDescriptor);

    pService->addCharacteristic(&altitudeCharacteristic);
    altitudeDescriptor.setValue("BME280 Altitude");
    altitudeCharacteristic.addDescriptor(&altitudeDescriptor);

    pService->addCharacteristic(&humidityCharacteristic);
    humidityDescriptor.setValue("BME280 Humidity");
    humidityCharacteristic.addDescriptor(&humidityDescriptor);

    pService->start();
    xTaskCreate(BME280_BLE::serviceTask,
                "BME280 BLE Task",
                BME280_SERVICE_TASK_STACK_DEPTH,
                NULL,
                tskIDLE_PRIORITY,
                &taskHandle);
}

void BME280_BLE::startService()
{
    pService->start();
    vTaskResume(taskHandle);
}

void BME280_BLE::stopService()
{
    vTaskSuspend(taskHandle);
    pService->stop();
}