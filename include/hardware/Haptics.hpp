#pragma once

#include <Arduino.h>

#include <list>
#include <mutex>

using namespace std;

uint8_t scale(uint8_t n, uint8_t a, uint8_t b, uint8_t c, uint8_t d);

struct Vibration
{
    uint8_t amplitude;
    uint32_t duration;

    Vibration(uint8_t amplitude, uint32_t duration);
};

struct VibrationEffect
{
    static VibrationEffect* CLICK;
    static VibrationEffect* DOUBLE_CLICK;

    list<Vibration> composition;

    VibrationEffect(){};

    VibrationEffect(list<Vibration> composition);
};

class Haptics
{
  private:
    static const uint8_t OUTPUT_PIN = A1;

    static Haptics* instance;

    std::mutex mutex;

    Haptics();

    void vibrate(uint8_t amplitude, uint32_t millisecconds);

  public:
    static Haptics* get();

    void play(VibrationEffect* effect);

    void aquireLock();

    void releaseLock();
};