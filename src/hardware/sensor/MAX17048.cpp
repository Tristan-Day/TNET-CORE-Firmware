#include <hardware/sensor/MAX17048.hpp>

MAX17048* MAX17048::instance = nullptr;

MAX17048::MAX17048()
{
    sensor = new Adafruit_MAX17048();
    sensor->begin();

    preferences = new Preferences();
    preferences->begin("MAX17048");

    CHG = new Persistent<long>(preferences, "CHG");
    VLT = new Persistent<float>(preferences, "VLT");

    auto lambda = [&]()
    {
        delay(500);

        if (sensor->cellPercent() != 0)
        {
            CHG->set(sensor->cellPercent());
        }

        if (sensor->cellVoltage() != 0)
        {
            VLT->set(sensor->cellVoltage());
        }
    };

    processor = new Microservice(lambda, DEFAULT_INTERVAL);
    processor->start("BAT", 3000, 1);
}

MAX17048* MAX17048::get()
{
    if (instance == nullptr)
    {
        instance = new MAX17048();
    }
    return instance;
}