#pragma once

#include <service/Service.hpp>
#include <hardware/Haptics.hpp>

struct TouchEvent
{
    static constexpr uint8_t NONE = 0b00000000;

    static constexpr uint8_t LONG_PRESS = 0b00000001;
    static constexpr uint8_t DOUBLE_PRESS = 0b00000010;
};

class Touch : public Service
{
  private:
    static constexpr uint8_t INPUT_PIN = 5;
    static constexpr uint16_t THRESHOLD = 1500;

    static constexpr uint16_t LONG_PRESS = 2000;
    static constexpr uint8_t SHORT_PRESS = 15;

    static constexpr uint16_t MAX_INTERVAL = 3000;

    static Touch* instance;

    EventGroupHandle_t interruptEvent;
    uint32_t lastEvent;

    Touch();

    static void eventInterrupt();

    void handleEvent(TouchEvent* event);

    void execute() override;

    std::string getName() override;

  public:
    EventGroupHandle_t touchEvent;

    static void init();

    static Touch* get();

    Touch(Touch& other) = delete;

    void operator=(const Touch&) = delete;
};