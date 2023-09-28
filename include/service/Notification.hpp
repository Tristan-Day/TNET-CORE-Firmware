#pragma once

#include <service/Microservice.hpp>

#include <hardware/Bluetooth.hpp>
#include <hardware/sensor/Touch.hpp>

class NotificationService
{
    static constexpr const char* SERVICE_UUID =
        "25fb6509-75fc-4686-bbec-163d5c05ed8a";

    static constexpr const char* SERVER_NOTIFIY_UUID =
        "88c7bb0c-eed8-4964-9034-cf8f70e82158";

    BLEService* service;
    Microservice* eventProcessor;

    // @brief Output Characteristic - Server to Client
    BLECharacteristic* OUT;

  public:
    NotificationService();
};