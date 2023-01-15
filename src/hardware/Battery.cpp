#include <hardware/Battery.hpp>

unsigned long Battery::sampleInterval;

void Battery::init(unsigned long sampleInterval)
{
    pinMode(VOLTAGE_INPUT_PIN, INPUT);
    Battery::sampleInterval = sampleInterval;
}

uint8_t Battery::getChargePercent() { return 100; }