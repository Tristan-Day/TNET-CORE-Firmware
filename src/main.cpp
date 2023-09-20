#include <Arduino.h>

#include <hardware/Bluetooth.hpp>

#include <hardware/sensor/Touch.hpp>
#include <hardware/sensor/BME688.hpp>

#include <service/Environment.hpp>
#include <service/Notification.hpp>

EnvironmentService* environmentService;
NotificationService* notificationService;

void setup()
{
    pinMode(I2C_POWER, OUTPUT);
    pinMode(NEOPIXEL_POWER, OUTPUT);

    digitalWrite(I2C_POWER, LOW);
    digitalWrite(NEOPIXEL_POWER, LOW);

    delay(5000);

    Bluetooth::init();
    Touch::init();

    environmentService = new EnvironmentService();
    environmentService->referesh();

    notificationService = new NotificationService();
    notificationService->start();

    Bluetooth::get()->advertising->start();

}

void loop()
{
    environmentService->referesh();

    vTaskDelay(30000);
}
