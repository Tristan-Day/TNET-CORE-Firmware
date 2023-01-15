#pragma once

#include <Arduino.h>

#define LONG_HOLD_MIN 2000
#define LONG_HOLD_MAX 5000

#define DOUBLE_TAP_MAX 1000

struct TouchEvent
{
    enum
    {
        NONE,
        LONG_HOLD,
        DOUBLE_TAP
    };

    unsigned long timestamp;
    unsigned long duration;

    TouchEvent() : timestamp(0), duration(0) {};

    TouchEvent(unsigned long timestamp, unsigned long duration)
        : timestamp(timestamp), duration(duration) {};
};

class TouchSense
{
private:
    static const uint8_t INPUT_PIN = 15;
    static const uint16_t TASK_STACK_DEPTH = 2000;
    static const uint8_t CAPACITANCE_THRESHOLD = 20;

    // Signals when the ISR is called
    static EventGroupHandle_t eventInterruptGroup;

    static void interrupt();

    static void eventTimer(void *);

    static void eventProcessor(TouchEvent event);

public:
    // Signals a processed touch event
    static EventGroupHandle_t touchEventGroup;
    static TaskHandle_t taskHandle;

    static void init();
};