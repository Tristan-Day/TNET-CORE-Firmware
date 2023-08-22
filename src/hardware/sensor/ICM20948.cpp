#include <hardware/sensor/ICM20948.hpp>

ICM20948* ICM20948::instance = nullptr;

ICM20948::ICM20948()
{
    sensor = new Sparkfun_ICM_20948_I2C();
    sensor->begin();

#ifdef DEBUG
    sensor->enableDebugging();
#endif
}

ICM20948* ICM20948::get()
{
    if (instance == nullptr)
    {
        instance = new ICM20948();
    }
    return instance;
}