#include <hardware/sensor/BME280.hpp>

BME280 *BME280::pInstance;

BME280 *BME280::get()
{
    if (pInstance == NULL)
    {
        pInstance = new BME280();
    }
    return pInstance;
}

void BME280::init(unsigned long sampleInterval)
{
    bool connectionState = Adafruit_BME280::begin(ADDRESS);

    if (not connectionState)
    {
        Serial.println("[ERROR] BME280 not found");
        return;
    }

    Adafruit_BME280::setSampling(Adafruit_BME280::MODE_FORCED);

    this->sampleInterval = sampleInterval;
}