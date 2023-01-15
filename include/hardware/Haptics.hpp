#pragma once

#include <Arduino.h>
#include <list>
#include <mutex>

using namespace std;

struct VibrationEffect
{
    static VibrationEffect *TICK;

    list<uint16_t> timings;
    uint8_t repeat;

    VibrationEffect(list<uint16_t> timings, uint8_t repeat)
        : timings(timings), repeat(repeat) {};
};

class Haptics
{
private:
    static const uint8_t OUTPUT_PIN = LED_BUILTIN;

    static std::mutex mutex;

public:
    static void init();

    static void vibrate(long millisecconds);

    static void vibrate(VibrationEffect *effect);

    static void aquireLock();

    static void releaseLock();
};