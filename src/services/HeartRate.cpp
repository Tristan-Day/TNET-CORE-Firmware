#include <services/HeartRate.hpp>

namespace HeartRateService
{
    BLEService *pService;
    TaskHandle_t taskHandle;

    BLECharacteristic heartRateCharacteristic(
        BLEUUID((uint16_t)0x2A37),
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY);

    BLECharacteristic sensorLocationCharacteristic(
        BLEUUID((uint16_t)0x2A38),
        BLECharacteristic::PROPERTY_READ);
    BLEDescriptor sensorLocationDescriptor(BLEUUID((uint16_t)0x2901));

    BLECharacteristic sensorTemperatureCharacteristic(
        BLEUUID((uint16_t)0x2A6E),
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY);
    BLEDescriptor sensorTemperatureDescriptor(BLEUUID((uint16_t)0x2901));

    const uint8_t sensorLocation = 0x1;
}

void HeartRateService::createService(BLEServer *pServer, BLEAdvertising *pAdvertising)
{
    if (not MAX30105::connectionState) {return;}

    pService = pServer->createService(BLEUUID((uint16_t)0x180D));
    pAdvertising->addServiceUUID(BLEUUID((uint16_t)0x180D));

    pService->addCharacteristic(&heartRateCharacteristic);
    heartRateCharacteristic.addDescriptor(new BLE2902());

    pService->addCharacteristic(&sensorLocationCharacteristic);
    sensorLocationDescriptor.setValue("Sensor Location");
    sensorLocationCharacteristic.addDescriptor(&sensorLocationDescriptor);

    pService->addCharacteristic(&sensorTemperatureCharacteristic);
    sensorLocationDescriptor.setValue("Sensor Temperature");
    sensorLocationCharacteristic.addDescriptor(&sensorLocationDescriptor);
    sensorTemperatureCharacteristic.addDescriptor(new BLE2902());

    pService->start();

    xTaskCreate(serviceTask,
                "Heart Rate Service",
                HEART_RATE_SERVICE_TASK_STACK_DEPTH,
                NULL,
                tskIDLE_PRIORITY,
                &taskHandle);
    
    vTaskSuspend(taskHandle);
}

void HeartRateService::serviceTask(void*)
{
    sensorLocationCharacteristic.setValue((uint8_t *)&sensorLocation, 1);
    uint8_t heartRateData[8];

    while (true)
    {
        if (MAX30105::contactDetected)
        {
            heartRateData[0] = 0b00001110;
        }
        else
        {
            heartRateData[0] = 0b00001100;
        }
        heartRateData[1] = MAX30105::getHeartRate();

        heartRateCharacteristic.setValue(heartRateData, 8);
        heartRateCharacteristic.notify();

        uint16_t sensorTemperature = MAX30105::device.readTemperature() * 100;

        sensorTemperatureCharacteristic.setValue((uint8_t *)&sensorTemperature, 2);
        sensorTemperatureCharacteristic.notify();

        delay(MAX30105::sampleInterval);
    }
}