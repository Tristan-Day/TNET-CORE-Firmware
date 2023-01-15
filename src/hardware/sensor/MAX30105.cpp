#include <hardware/sensor/MAX30105.hpp>

MAX30105 *MAX30105::pInstance;

void MAX30105::task(void *)
{
    MAX30105 *self = get();

    unsigned long lastPulse = 0;
    uint8_t bufferCounter = 0;

    while (true)
    {
        xSemaphoreTake(self->taskSemaphore, portMAX_DELAY);
        self->Sparkfun_MAX30105::wakeUp();

        for (unsigned long i = 0; i < SAMPLE_SIZE; i++)
        {
            uint32_t irSample = self->Sparkfun_MAX30105::getIR();
            if (irSample < 2000)
            {
                self->contactDetected = false;
            }
            else if (checkForBeat(irSample))
            {
                self->contactDetected = true;

                unsigned long delta = millis() - lastPulse;
                lastPulse = millis();

                float bpm = 60 / (delta / 1000.0);
                if (bpm < 255 and bpm > 20)
                {
                    self->buffer[bufferCounter++] = (uint8_t)bpm;
                    bufferCounter %= BUFFER_SIZE;
                }
            }
        }
        self->Sparkfun_MAX30105::shutDown();
        xSemaphoreGive(self->taskSemaphore);
        delay(self->sampleInterval);
    }
}

MAX30105 *MAX30105::get()
{
    if (pInstance == NULL)
    {
        pInstance = new MAX30105();
    }
    return pInstance;
}

void MAX30105::init(unsigned long sampleInterval)
{
    bool connectionState = Sparkfun_MAX30105::begin(Wire, I2C_SPEED_STANDARD);

    if (not connectionState)
    {
        Serial.println("[ERROR] MAX30105 not found");
        return;
    }
    Sparkfun_MAX30105::setup();

    this->sampleInterval = sampleInterval;
    taskSemaphore = xSemaphoreCreateBinary();

    xTaskCreate(task, "MAX30105 Service", TASK_STACK_DEPTH, NULL, tskIDLE_PRIORITY,
        &taskHandle);

    xSemaphoreGive(taskSemaphore);
}

uint8_t MAX30105::getHeartRate()
{
    unsigned long sum = 0;
    for (uint8_t i = 0; i < BUFFER_SIZE; i++)
    {
        sum += buffer[i];
    }
    return sum / BUFFER_SIZE;
}

float MAX30105::getTemperature() { return Sparkfun_MAX30105::readTemperature(); }