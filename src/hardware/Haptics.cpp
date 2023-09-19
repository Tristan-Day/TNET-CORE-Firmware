#include <hardware/Haptics.hpp>

uint8_t scale(uint8_t n, uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    if (n <= a)
        return c;

    if (n >= b)
        return d;

    // Calculate the proportion of n within the range [a, b]
    double proportion = static_cast<double>(n - a) / (b - a);

    // Scale the proportion to the range [e, f]
    return c + proportion * (d - c);
}

Vibration::Vibration(uint8_t amplitude, uint32_t duration)
{
    this->amplitude = amplitude;
    this->duration = duration;
}

VibrationEffect* VibrationEffect::CLICK =
     new VibrationEffect({{100, 250}});

VibrationEffect* VibrationEffect::DOUBLE_CLICK =
    new VibrationEffect({{100, 250}, {0, 250}, {100, 250}});

VibrationEffect::VibrationEffect(list<Vibration> composition) : composition(composition)
{
    this->composition = composition;
}

Haptics* Haptics::instance = nullptr;

Haptics::Haptics()
{
    pinMode(OUTPUT_PIN, OUTPUT);
}

void Haptics::vibrate(uint8_t amplitude, uint32_t millisecconds)
{
    if (amplitude == 0)
    {
        analogWrite(OUTPUT_PIN, 0);
    }
    else
    {
        // Reduce peak stating current
        if (amplitude > 85)
        {
            analogWrite(OUTPUT_PIN, amplitude * 0.75);
            delay(25);
        }

        analogWrite(OUTPUT_PIN, scale(amplitude, 0, 100, 190, 255));
    }

    delay(millisecconds);
}

Haptics* Haptics::get()
{
    if (instance == NULL)
    {
        instance = new Haptics();
    }
    return instance;
}

void Haptics::play(VibrationEffect* effect)
{
    aquireLock();

    for (Vibration vibration : effect->composition)
    {
        vibrate(vibration.amplitude, vibration.duration);
    }

    analogWrite(OUTPUT_PIN, 0);
    releaseLock();
}

void Haptics::aquireLock()
{
    mutex.lock();
}

void Haptics::releaseLock()
{
    mutex.unlock();
}