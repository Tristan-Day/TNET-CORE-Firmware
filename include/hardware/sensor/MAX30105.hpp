#pragma once

#include <service/Service.hpp>

#include <Sparkfun_MAX30105.h>
#include <HeartRateAlgorithm.h>

class MAX30105 : public Service
{
  private:
    static constexpr uint16_t BUFFER_SIZE = 4;
    static constexpr uint16_t SAMPLE_SIZE = 1000;

    static MAX30105* instance;

    Sparkfun_MAX30105* sensor;
    uint8_t buffer[BUFFER_SIZE];

    MAX30105();

    void execute() override;

    uint32_t getStackDepth() override;

    string getName() override;

  public:
    static MAX30105* get();

    MAX30105(MAX30105& other) = delete;

    void operator=(const MAX30105&) = delete;

    uint8_t getHeartRate();

    float getTemperature();

    bool hasContact();
};