#include <hardware/sensor/MAX30105.hpp>

MAX30105* MAX30105::instance = nullptr;

MAX30105::MAX30105()
{
    sensor = new Sparkfun_MAX30105();

    sensor->begin();
    sensor->setup();
}

void MAX30105::execute()
{
    uint32_t lastPulse = 0;
    uint8_t bufferCounter = 0;

    sensor->wakeUp();

    for (uint16_t i = 0; i < SAMPLE_SIZE; i++)
    {
        uint32_t sample = sensor->getIR();

        if (hasContact() && checkForBeat(sample))
        {
            unsigned long delta = millis() - lastPulse;
            lastPulse = millis();

            float bpm = 60 / (delta / 1000.0);
            if (bpm < 255 and bpm > 20)
            {
                buffer[bufferCounter++] = (uint8_t)bpm;
                bufferCounter %= BUFFER_SIZE;
            }
        }
    }

    sensor->shutDown();
}

uint32_t MAX30105::getStackDepth()
{
    return 2000;
}

string MAX30105::getName()
{
    return "MAX30105 Service";
}

MAX30105* MAX30105::get()
{
    if (instance == nullptr)
    {
        instance = new MAX30105();
        instance->start();
    }
    return instance;
}

uint8_t MAX30105::getHeartRate()
{
    uint32_t sum = 0;
    for (uint8_t i = 0; i < BUFFER_SIZE; i++)
    {
        sum += buffer[i];
    }
    return sum / BUFFER_SIZE;
}

float MAX30105::getTemperature()
{
    return sensor->readTemperature();
}

bool MAX30105::hasContact()
{
    uint32_t sample = sensor->getIR();

    if (sample < 2000)
    {
        return false;
    }
    return true;
}
