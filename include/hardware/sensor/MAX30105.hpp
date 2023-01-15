#pragma once

#include <SystemTask.hpp>
#include <Sparkfun_MAX30105.h>
#include <HeartRateAlgorithm.h>

class MAX30105 : public SystemTask, Sparkfun_MAX30105
{
private:
    static const uint16_t BUFFER_SIZE = 4;
    static const uint16_t SAMPLE_SIZE = 1000;
    static const uint16_t TASK_STACK_DEPTH = 2000;

    static MAX30105 *pInstance;

    uint8_t buffer[BUFFER_SIZE];
    uint8_t bufferCounter;
    uint32_t lastPulse;

    MAX30105() { }

    static void task(void *);

public:
    unsigned long sampleInterval;
    bool contactDetected;

    MAX30105(const MAX30105 &obj) = delete;

    static MAX30105 *get();

    void init(unsigned long sampleInterval);

    uint8_t getHeartRate();

    float getTemperature();
};