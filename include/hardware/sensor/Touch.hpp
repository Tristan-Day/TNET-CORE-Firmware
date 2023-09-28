#pragma once

#include <service/Microservice.hpp>
#include <hardware/Haptics.hpp>

struct TouchEvent
{
    static constexpr uint8_t LONG_PRESS = 0b00000001;
    static constexpr uint8_t DOUBLE_PRESS = 0b00000010;
};

class Touch
{
  private:
    static constexpr uint8_t INPUT_PIN = 5;
    static constexpr uint16_t THRESHOLD = 1500;

    static constexpr uint16_t LONG_PRESS = 2000;
    static constexpr uint8_t SHORT_PRESS = 15;

    static Touch* instance;

    EventGroupHandle_t interruptEvent;
    Microservice* eventProcessor;

    uint32_t lastEvent;

    Touch();

    static void interrupt();

  public:
    EventGroupHandle_t touchEvent;

    static void init();

    static Touch* get();

    Touch(Touch& other) = delete;

    void operator=(const Touch&) = delete;
};