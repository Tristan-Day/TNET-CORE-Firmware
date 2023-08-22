#include <hardware/sensor/MAX17048.hpp>

MAX17048* MAX17048::instance = nullptr;

MAX17048::MAX17048()
{
    sensor = new Adafruit_MAX17048();
    sensor->begin();
}

MAX17048* MAX17048::get()
{
    if (instance == nullptr)
    {
        instance = new MAX17048();
    }
    return instance;
}