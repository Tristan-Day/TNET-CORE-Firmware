#include <Arduino.h>

#include <Extras.hpp>

#include <hardware/Bluetooth.hpp>
#include <hardware/sensor/Touch.hpp>
#include <hardware/sensor/BME688.hpp>

#include <service/Environment.hpp>
#include <service/Notification.hpp>

EnvironmentService* environmentService;
NotificationService* notificationService;

void setup()
{
    delay(5000);

    pinMode(I2C_POWER, OUTPUT);
    digitalWrite(I2C_POWER, LOW);

    pinMode(NEOPIXEL_POWER, OUTPUT);
    digitalWrite(NEOPIXEL_POWER, LOW);

#ifdef DEBUG

    Serial.printf("[INFO] Core0 LRR: '%s'\n",
                  getResetReason(esp_rom_get_reset_reason(0)));

    Serial.printf("[INFO] Core1 LRR: '%s'\n",
                  getResetReason(esp_rom_get_reset_reason(1)));
#endif

    Bluetooth::init();
    Touch::init();

    environmentService = new EnvironmentService();
    notificationService = new NotificationService();

    Bluetooth::get()->advertising->start();
}

void loop()
{
    environmentService->referesh();

    vTaskDelay(30000);
}
