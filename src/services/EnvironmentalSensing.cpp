#include <services/EnvironmentalSensing.hpp>

EnvironmentService *EnvironmentService::pInstance;

void EnvironmentService::task(void *)
{
    EnvironmentService *self = EnvironmentService::get();
    BME280 *sensor = BME280::get();

    while (true)
    {
        xSemaphoreTake(self->taskSemaphore, portMAX_DELAY);

        bool readSuccess = sensor->takeForcedMeasurement();
        if (not readSuccess)
        {
            Serial.println("[ERROR] BME280 - Failed to take measurement");
            return;
        }

        delay(1000);

        uint16_t temperature = sensor->readTemperature() * 100;
        self->temperatureCharacteristic.setValue((uint8_t *)&temperature, 2);
        self->temperatureCharacteristic.notify();

        uint32_t pressure = sensor->readPressure() * 10;
        self->pressureCharacteristic.setValue((uint8_t *)&pressure, 4);
        self->pressureCharacteristic.notify();

        uint16_t altitude = sensor->readAltitude(SEA_LEVEL_PRESSURE) * 10;
        self->altitudeCharacteristic.setValue((uint8_t *)&altitude, 2);
        self->altitudeCharacteristic.notify();

        uint16_t humidity = sensor->readHumidity() * 100;
        self->humidityCharacteristic.setValue((uint8_t *)&humidity, 2);
        self->humidityCharacteristic.notify();

        xSemaphoreGive(self->taskSemaphore);
        delay(sensor->sampleInterval);
    }
}

EnvironmentService *EnvironmentService::get()
{
    if (pInstance == NULL)
    {
        pInstance = new EnvironmentService();
    }
    return pInstance;
}

void EnvironmentService::init(BLEServer *pServer)
{
    pService = pServer->createService(BLEUUID((uint16_t)0x181A));
    taskSemaphore = xSemaphoreCreateBinary();

    pService->addCharacteristic(&temperatureCharacteristic);
    temperatureCharacteristic.addDescriptor(new BLE2902());

    pService->addCharacteristic(&pressureCharacteristic);
    pressureCharacteristic.addDescriptor(new BLE2902());

    pService->addCharacteristic(&altitudeCharacteristic);
    altitudeCharacteristic.addDescriptor(new BLE2902());

    pService->addCharacteristic(&humidityCharacteristic);
    humidityCharacteristic.addDescriptor(new BLE2902());

    pService->start();

    xTaskCreate(task, "Environmental Sensing Service", TASK_STACK_DEPTH, NULL,
        tskIDLE_PRIORITY, &taskHandle);

    xSemaphoreGive(taskSemaphore);
}