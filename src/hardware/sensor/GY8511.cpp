#include <hardware/sensor/GY8511.hpp>

TaskHandle_t GY8511::taskHandle = NULL;
unsigned long GY8511::sampleInterval;

void GY8511::task(void *)
{
    while (true)
    {
        for (uint8_t i = 0; i < BUFFER_SIZE; i++)
        {
            digitalWrite(ENABLE_PIN, HIGH);
            buffer[i][0] = analogRead(REFERENCE_PIN);
            buffer[i][1] = analogRead(INPUT_PIN);
            digitalWrite(ENABLE_PIN, LOW);

            delay(100);
        }
        delay(sampleInterval);
    }
}

void GY8511::init(unsigned long sampleInterval)
{
    pinMode(INPUT_PIN, INPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(REFERENCE_PIN, INPUT);

    xTaskCreate(task, "GY8511 Service", TASK_STACK_DEPTH, NULL,
        tskIDLE_PRIORITY, &taskHandle);

    GY8511::sampleInterval = sampleInterval;
}

float GY8511::getIntensity()
{
    long referenceVoltage = 0;
    float inputVoltage = 0;

    for (uint8_t i = 0; i < BUFFER_SIZE; i++)
    {
        referenceVoltage += buffer[i][0];
        inputVoltage += buffer[i][1];
    }

    inputVoltage = 3.3 / referenceVoltage * inputVoltage;

    return (inputVoltage - 0.99) * 15.0 / 14.01;
}