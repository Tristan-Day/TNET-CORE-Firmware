#include <Arduino.h>
#include <Wire.h>

// clang-format off
#include <hardware/Bluetooth.hpp>
#include <hardware/Battery.hpp>
#include <hardware/Haptics.hpp>
#include <hardware/Touch.hpp>

#include <hardware/sensor/BME280.hpp>
#include <hardware/sensor/MAX30105.hpp>

#include <services/PowerManagement.hpp>
#include <services/EnvironmentalSensing.hpp>
#include <services/Notification.hpp>
#include <services/HeartRate.hpp>
#include <services/Metronome.hpp>
// clang-format on

// #define DEBUG

const uint16_t BASE_SAMPLE_INTERVAL = 10000;

void setup()
{
    Serial.begin(115200);
    Wire.begin(23, 19);

    delay(2500);

    // Bluetooth
    Bluetooth::get()->init("TNET-CORE");
    Bluetooth::get()->startAdvertising();

    // Battery
    Battery::init(BASE_SAMPLE_INTERVAL);

    // Haptics
    Haptics::init();
    Haptics::vibrate(VibrationEffect::TICK);

    // Touch Sense
    TouchSense::init();

    // MAX30105 Heart Rate Sensor
    MAX30105::get()->init(BASE_SAMPLE_INTERVAL);

    // BME280 Environmental Sensor
    BME280::get()->init(BASE_SAMPLE_INTERVAL);

    // Initialise Bluetooth Services
    BLEServer *pServer = Bluetooth::get()->pServer;

    PowerManagementService::get()->init(pServer);
    NotificationService::get()->init(pServer);
    EnvironmentService::get()->init(pServer);
    MetronomeService::get()->init(pServer);

    HeartRateService::get()->init(pServer, Bluetooth::get()->pAdvertising);
}

void loop()
{
#ifdef DEBUG

    // clang-format off
    const String tasks[6] = {
        "Bluetooth Event Handler",
        "Battery Monitoring Service",
        "Metronome Service",
        "Environmental Sensing Service",
        "Touch Sense Task"
    };
    // clang-format on

    for (uint8_t i = 0; i < 6; i++)
    {
        TaskHandle_t task = xTaskGetHandle(tasks[i].c_str());

        if (task != NULL)
        {
            uint32_t watermark = uxTaskGetStackHighWaterMark(task);
            Serial.printf("Task [%s] Stack Watermark: %i", tasks[i], watermark);
        }
    }
#endif

    delay(10000);
}