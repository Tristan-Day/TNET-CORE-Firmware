#include <Arduino.h>

// #include <script/Startup.hpp>

#include <hardware/sensor/Touch.hpp>
#include <hardware/sensor/BME688.hpp>

#include <service/Battery.hpp>
#include <service/Environment.hpp>
// #include <service/Notification.hpp>

BatteryService* BAT;
EnvironmentService* ENV;
// NotificationService*  HID;

void setup()
{
    delay(5000);

    pinMode(I2C_POWER, OUTPUT);
    digitalWrite(I2C_POWER, LOW);

    pinMode(NEOPIXEL_POWER, OUTPUT);
    digitalWrite(NEOPIXEL_POWER, LOW);

    Touch::get()->enable();
}

void loop()
{
    Bluetooth::get()->enable();

    ENV = new EnvironmentService();
    BAT = new BatteryService();

    Bluetooth::get()->advertising->start();

    ENV->refresh();
    BAT->refresh();

    vTaskDelay(pdMS_TO_TICKS(30000));

    delete ENV;
    delete BAT;

    Bluetooth::get()->disable();

    vTaskDelay(pdMS_TO_TICKS(60000));

#ifdef DEBUG
    Serial.println(esp_get_free_internal_heap_size());
#endif
}
