#include <hardware/sensor/BME688.hpp>

BME688* BME688::instance = nullptr;

BME688::BME688()
{
    sensor = new BSEC2();
    sensor->begin(ADDRESS, Wire);

    bsec_virtual_sensor_t attributes[] = 
    {
        BSEC_OUTPUT_RAW_PRESSURE, 
        BSEC_OUTPUT_CO2_EQUIVALENT,
        BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE
    };

    sensor->updateSubscription(attributes, 5, BSEC_SAMPLE_RATE_LP);

    preferences = new Preferences();
    preferences->begin("BME688");

    TMP = new Persistent<float>(preferences, "TMP");
    PRE = new Persistent<float>(preferences, "PRE");
    HUM = new Persistent<float>(preferences, "HIM");
    ALT = new Persistent<float>(preferences, "ALT");
    CO2 = new Persistent<float>(preferences, "CO2");
    VOC = new Persistent<float>(preferences, "VOC");

    auto lambda = [&]() 
    {
        sensor->run();

        TMP->set(sensor->temperature);

        PRE->set(sensor->pressure);
        HUM->set(sensor->humidity);

        float atmospheric = sensor->pressure / 100.0F;
        ALT->set(44330.0 * (1.0 - pow(atmospheric / SEA_LEVEL_PRESSURE, 0.1903)));

        CO2->set(sensor->co2Equivalent);
        VOC->set(sensor->breathVocEquivalent);
    };

    processor = new Microservice(lambda, DEFAULT_INTERVAL);
    processor->start("BME", 3000, 1);
}

BME688* BME688::get()
{
    if (instance == nullptr)
    {
        instance = new BME688();
    }
    return instance;
}
