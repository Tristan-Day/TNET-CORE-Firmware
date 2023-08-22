#pragma once

#include <service/Service.hpp>
#include <Adafruit_BME680.h>

class BME688 : public Service
{
  private:
    static constexpr float TEMPERATURE_OFFSET = 5.83;
    static constexpr uint8_t ADDRESS = 0x76;

    static BME688* instance;

    Adafruit_BME680* sensor;

    BME688();

    void execute() override;

    uint32_t getStackDepth() override;

    string getName() override;

  public:
    static BME688* get();

    BME688(BME688& other) = delete;

    void operator=(const BME688&) = delete;

    float getTemperature();

    float getPressure();

    float getHumidity();

    float getAltitude();
};