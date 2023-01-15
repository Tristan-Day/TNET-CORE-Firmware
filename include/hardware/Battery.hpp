#pragma once

#include <Arduino.h>

class Battery
{
private:
    static const uint8_t VOLTAGE_INPUT_PIN = 13;

public:
    static unsigned long sampleInterval;

    static void init(unsigned long sampleInterval);

    static uint8_t getChargePercent();
};