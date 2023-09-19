#pragma once

#include <BSEC2.hpp>

#include <Arduino.h>
#include <Persistence.hpp>

#include <service/Service.hpp>

class BME688 : public Service
{
  private:
    static constexpr uint8_t ADDRESS = 0x76;
    static constexpr float SEA_LEVEL_PRESSURE = 1013.25;

    static BME688* instance;

    BSEC2* sensor;

    Preferences* preferences;

    BME688();

    void execute() override;

    uint32_t getStackDepth() override;

    string getName() override;

  public:
    Persistent<float>* TMP;

    Persistent<float>* PRE;

    Persistent<float>* ALT;

    Persistent<float>* HUM;

    Persistent<float>* CO2;

    Persistent<float>* VOC;

    static BME688* get();

    BME688(BME688* other) = delete;

    void operator=(const BME688&) = delete;
};