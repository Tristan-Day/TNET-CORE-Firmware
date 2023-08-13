#include <Arduino.h>

#include <hardware/Bluetooth.hpp>
#include <hardware/PowerManager.hpp>

#include <hardware/sensor/BME688.hpp>
#include <hardware/sensor/GNSS.hpp>

#include <service/Battery.hpp>
#include <service/Environment.hpp>

void setup()
{
    delay(5000);

    pinMode(I2C_POWER, OUTPUT);
    pinMode(PIN_NEOPIXEL, OUTPUT);

    digitalWrite(I2C_POWER, LOW);
    digitalWrite(PIN_NEOPIXEL, LOW);

    Bluetooth::init();
    PowerManager::init();

    Console* console = new Console();

    console->addCommand("ECHO", [](string input) 
    {
        return input;
    });

    console->addCommand("PROFILE", [&](string profile) 
    {
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
}