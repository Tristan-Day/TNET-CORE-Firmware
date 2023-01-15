#pragma once

#include <Arduino.h>
#include <Adafruit_BME280.h>

class BME280 : public Adafruit_BME280
{
private:
    static const uint8_t ADDRESS = 0x76;

    static BME280 *pInstance;

    BME280() { }

public:
    unsigned long sampleInterval;

    BME280(const BME280 &obj) = delete;

    static BME280 *get();

    void init(unsigned long sampleInterval);
};