#include <services/HeartRate.hpp>

HeartRateService *HeartRateService::pInstance;

void HeartRateService::task(void *)
{
    HeartRateService *self = HeartRateService::get();
    MAX30105 *sensor = MAX30105::get();

    const uint8_t sensorLocation = HeartRateService::SENSOR_LOCATION;
    self->sensorLocationCharacteristic.setValue((uint8_t *)&sensorLocation, 1);

    uint8_t heartRateData[8];

    while (true)
    {
        xSemaphoreTake(self->taskSemaphore, portMAX_DELAY);

        if (sensor->contactDetected)
        {
            heartRateData[0] = 0b00001110;
        }
        else
        {
            heartRateData[0] = 0b00001100;
        }
        heartRateData[1] = sensor->getHeartRate();

        self->heartRateCharacteristic.setValue(heartRateData, 8);
        self->heartRateCharacteristic.notify();

        // clang-format off
        uint16_t sensorTemperature = sensor->getTemperature() * 100;
        self->sensorTemperatureCharacteristic.setValue((uint8_t *)&sensorTemperature, 2);
        // clang-format on

        xSemaphoreGive(self->taskSemaphore);
        delay(sensor->sampleInterval);
    }
}

HeartRateService *HeartRateService::get()
{
    if (pInstance == NULL)
    {
        pInstance = new HeartRateService();
    }
    return pInstance;
}

void HeartRateService::init(BLEServer *pServer, BLEAdvertising *pAdvertising)
{
    pService = pServer->createService(BLEUUID((uint16_t)0x180D));
    pAdvertising->addServiceUUID(BLEUUID((uint16_t)0x180D));
    taskSemaphore = xSemaphoreCreateBinary();

    pService->addCharacteristic(&heartRateCharacteristic);
    heartRateCharacteristic.addDescriptor(new BLE2902);

    pService->addCharacteristic(&sensorLocationCharacteristic);
    sensorLocationDescriptor.setValue("Sensor Location");
    sensorLocationCharacteristic.addDescriptor(&sensorLocationDescriptor);

    pService->addCharacteristic(&sensorTemperatureCharacteristic);
    sensorTemperatureDescriptor.setValue("Sensor Temperature");
    sensorTemperatureCharacteristic.addDescriptor(&sensorTemperatureDescriptor);

    pService->start();

    xTaskCreate(task, "Heart Rate Sensing Service", TASK_STACK_DEPTH, NULL,
        tskIDLE_PRIORITY, &taskHandle);

    xSemaphoreGive(taskSemaphore);
}