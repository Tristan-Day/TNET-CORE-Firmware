#pragma once

#include <Arduino.h>

class GY8511
{
private:
    static const uint8_t INPUT_PIN = 26;
    static const uint8_t ENABLE_PIN = 27;
    static const uint8_t REFERENCE_PIN = 16;

    static const uint8_t BUFFER_SIZE = 8;
    static const uint16_t TASK_STACK_DEPTH = 2000;

    static uint16_t buffer[BUFFER_SIZE][2];

    static void task(void *);

public:
    static TaskHandle_t taskHandle;
    static unsigned long sampleInterval;

    static void init(unsigned long sampleInterval);

    static float getIntensity();
};