#include <hardware/Haptics.hpp>

VibrationEffect *VibrationEffect::TICK = new VibrationEffect({ 500 }, 1);

std::mutex Haptics::mutex;

void Haptics::init()
{
    pinMode(OUTPUT_PIN, OUTPUT);
    digitalWrite(OUTPUT_PIN, HIGH);
}

void Haptics::vibrate(long millisecconds)
{
    digitalWrite(OUTPUT_PIN, LOW);
    delay(millisecconds);
    digitalWrite(OUTPUT_PIN, HIGH);
}

void Haptics::vibrate(VibrationEffect *effect)
{
#ifdef DEBUG
    Serial.println("[--- BEGIN EFFECT CONFIG ---]");
    Serial.println(effect->timings.size());
    Serial.println(effect->repeat);
    Serial.println("[---- END EFFECT CONFIG ----]");
#endif

    for (uint8_t i = 0; i < effect->repeat; i++)
    {
        bool toggle = true;
        for (uint16_t const &timing : effect->timings)
        {
            if (toggle)
            {
                Haptics::vibrate(timing);
            }
            else
            {
                delay(timing);
            }
            toggle = !toggle;
        }
    }
}

void Haptics::aquireLock()
{
    mutex.lock();
}

void Haptics::releaseLock()
{
    mutex.unlock();
}