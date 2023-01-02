#include <MAX30105_BLE.hpp>

namespace MAX30105_HR
{
    MAX30105 *pParticleSensor;
    TaskHandle_t taskHandle;

    uint8_t buffer[MAX30105_HR_BUFFER_SIZE];
    uint8_t bufferCounter = 0;
    uint32_t lastPulse = 0;

    bool contactDetected;
    uint8_t average;
}

void MAX30105_HR::serviceTask(void *pParameters)
{
    while (true)
    {
        uint32_t irSample = pParticleSensor->getIR();
        if (irSample < 2000)
        {
            contactDetected = false;
        }
        else if (checkForBeat(irSample))
        {
            contactDetected = true;

            long delta = millis() - lastPulse;
            lastPulse = millis();

            float bpm = 60 / (delta / 1000.0);
            if (bpm < 255 and bpm > 20)
            {
                buffer[bufferCounter++] = (uint8_t)bpm;
                bufferCounter %= MAX30105_HR_BUFFER_SIZE;
            }

            // #ifdef MAX30105_BLE_VERBOSE
            //             Serial.printf("[INFO] - Pulse Detected - BPM: %f "
            //                           "- IR Intensity %i\n",
            //                           bpm, irSample);
            // #endif

            uint16_t sum = 0;
            for (uint8_t i = 0; i < MAX30105_HR_BUFFER_SIZE; i++)
            {
                sum += buffer[i];
            }
            average = sum / MAX30105_HR_BUFFER_SIZE;

#ifdef MAX30105_BLE_VERBOSE
            Serial.printf("[INFO] HR Average: %i\n", average);
#endif
        }
        delay(MAX30NULL105_HR_INTERVAL);
    }
}

void MAX30105_HR::createService(MAX30105 *pParticleSensor)
{
    MAX30105_HR::pParticleSensor = pParticleSensor;

    xTaskCreate(serviceTask,
                "MAX30105 HR Service",
                MAX30105_HR_SERVICE_TASK_STACK_DEPTH,
                NULL,
                tskIDLE_PRIORITY,
                &taskHandle);
}

namespace MAX30105_BLE
{
    MAX30105 particleSensor;
    TaskHandle_t taskHandle;

    BLEService *pHeartRateService;
    BLECharacteristic heartRateCharacteristic(
        BLEUUID((uint16_t)0x2A37),
        BLECharacteristic::PROPERTY_READ);
    BLEDescriptor heartRateDescriptor(BLEUUID((uint16_t)0x2901));

    BLECharacteristic sensorLocationCharacteristic(
        BLEUUID((uint16_t)0x2A38),
        BLECharacteristic::PROPERTY_READ);
    BLEDescriptor sensorLocationDescriptor(BLEUUID((uint16_t)0x2901));

    BLEService *pBodyTemperatureService;
    BLECharacteristic bodyTemperatureCharacteristic(
        BLEUUID((uint16_t)0x2A6E),
        BLECharacteristic::PROPERTY_READ);
    BLEDescriptor bodyTemperatureDescriptor(BLEUUID((uint16_t)0x2901));

    bool connectionState;
    uint32_t sampleInterval;

    const uint8_t sensorLocation = 0x2;
}

void MAX30105_BLE::serviceTask(void *pParameters)
{
    while (true)
    {
        vTaskResume(MAX30105_HR::taskHandle);

#ifdef MAX30105_BLE_VERBOSE
        Serial.println("[INFO] HR Sampling Resumed");
#endif

        delay(MAX30105_BLE_SAMPLE_TIME);

        uint8_t heartRateData[8] = {0, MAX30105_HR::average, 0, 0, 0, 0, 0, 0};
        if (MAX30105_HR::contactDetected)
        {
            heartRateData[0] = 0b00001110;
        }
        else
        {
            heartRateData[0] = 0b00001100;
        }
        heartRateCharacteristic.setValue(heartRateData, 8);

        uint16_t bodyTemperature = particleSensor.readTemperature() * 100;
        bodyTemperatureCharacteristic.setValue((uint8_t *)&bodyTemperature, 2);

        vTaskSuspend(MAX30105_HR::taskHandle);

#ifdef MAX30105_BLE_VERBOSE
        Serial.println("[INFO] HR Sampling Paused");
#endif

        delay(sampleInterval);
    }
}

void MAX30105_BLE::createService(BLEServer *pServer, uint32_t sampleInterval)
{
    MAX30105_BLE::sampleInterval = sampleInterval;

    connectionState = particleSensor.begin(Wire, I2C_SPEED_STANDARD);
#ifdef MAX30105_BLE_VERBOSE
    if (not connectionState)
    {
        Serial.println("[INFO] MAX30105 not found");
    }
#endif
    particleSensor.setup();

    MAX30105_HR::createService(&particleSensor);

    pHeartRateService = pServer->createService(BLEUUID((uint16_t)0x180D));

    pHeartRateService->addCharacteristic(&heartRateCharacteristic);
    heartRateDescriptor.setValue("MAX30105 Heart Rate");
    heartRateCharacteristic.addDescriptor(&heartRateDescriptor);
    
    pHeartRateService->addCharacteristic(&sensorLocationCharacteristic);
    sensorLocationDescriptor.setValue("MAX30105 Sensor Location");
    sensorLocationCharacteristic.addDescriptor(&sensorLocationDescriptor);

    pBodyTemperatureService = pServer->createService(MAX30105_TEMPERATURE_SERVICE_UUID);

    pBodyTemperatureService->addCharacteristic(&bodyTemperatureCharacteristic);
    bodyTemperatureDescriptor.setValue("MAX30105 Temperature");
    bodyTemperatureCharacteristic.addDescriptor(&bodyTemperatureDescriptor);

    pHeartRateService->start();
    pBodyTemperatureService->start();

    sensorLocationCharacteristic.setValue((uint8_t*)&sensorLocation, 1);

    xTaskCreate(MAX30105_BLE::serviceTask,
                "MAX30105 BLE Service",
                MAX30105_BLE_SERVICE_TASK_STACK_DEPTH,
                NULL,
                tskIDLE_PRIORITY,
                &taskHandle);
}

void MAX30105_BLE::startService()
{
    pHeartRateService->start();
    vTaskResume(taskHandle);
}

void MAX30105_BLE::stopService()
{
    vTaskSuspend(taskHandle);
    pHeartRateService->stop();
}
