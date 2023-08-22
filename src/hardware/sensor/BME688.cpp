#include <hardware/sensor/BME688.hpp>

BME688* BME688::instance = nullptr;

BME688::BME688()
{
    sensor = new Adafruit_BME680(&Wire);
    sensor->begin(ADDRESS);

    sensor->setTemperatureOversampling(BME680_OS_4X);

    sensor->setHumidityOversampling(BME680_OS_4X);
    sensor->setPressureOversampling(BME680_OS_4X);

    sensor->setIIRFilterSize(BME68X_FILTER_SIZE_31);
    sensor->setGasHeater(320, 150);
}

void BME688::execute()
{
    sensor->performReading();
}

uint32_t BME688::getStackDepth()
{
    return 2500;
}

string BME688::getName()
{
    return "BME688 Service";
}

BME688* BME688::get()
{
    if (instance == nullptr)
    {
        instance = new BME688();
        instance->start();
    }
    return instance;
}

float BME688::getTemperature()
{
    return sensor->temperature - BME688::TEMPERATURE_OFFSET;
}

float BME688::getPressure()
{
    return sensor->pressure;
}

float BME688::getHumidity()
{
    return sensor->humidity;
}

float BME688::getAltitude()
{
    float atmospheric = sensor->pressure / 100.0F;

    return 44330.0 * (1.0 - pow(atmospheric / SENSORS_PRESSURE_SEALEVELHPA, 0.1903));
}
