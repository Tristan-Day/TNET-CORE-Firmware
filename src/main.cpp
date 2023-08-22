#include <Arduino.h>

#include <hardware/Bluetooth.hpp>
#include <hardware/PowerManager.hpp>

#include <hardware/sensor/ICM20948.hpp>
#include <hardware/sensor/MAX30105.hpp>
#include <hardware/sensor/BME688.hpp>
#include <hardware/sensor/GNSS.hpp>

#include <service/Console.hpp>

#include <service/Battery.hpp>
#include <service/Environment.hpp>

void setup()
{
    delay(5000);

#ifdef DEBUG
    Serial.println("[INFO] Disabling Unused Power Sources");
#endif

    pinMode(I2C_POWER, OUTPUT);
    pinMode(NEOPIXEL_POWER, OUTPUT);

    digitalWrite(I2C_POWER, LOW);
    digitalWrite(NEOPIXEL_POWER, LOW);

    delay(5000);

    Bluetooth::init();

    delay(5000);

#ifdef DEBUG
    Serial.println("[INFO] Starting Power Manager");
#endif

    PowerManager::init();

    delay(5000);

#ifdef DEBUG
    Serial.println("[INFO] Starting Bluetooth Services");
#endif

    Console* console = new Console();

    console->addCommand("ECHO", [](string input) { return input; });

    console->addCommand("PROFILE", [&](string profile) {
        uint8_t index = stoi(profile);

        if (index != PowerManager::get()->getProfile())
        {
            PowerManager::get()->setProfile(index);
        }

        return "✔️ Profile Set";
    });

    BatteryService* batteryService = new BatteryService();
    batteryService->start();

    EnvironmentService* environmentService = new EnvironmentService();
    environmentService->start();
}

void loop()
{
#ifdef PROFILE

#else
    vTaskDelete(NULL);
#endif
}