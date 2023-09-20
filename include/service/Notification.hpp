#pragma once

#include <service/Service.hpp>

#include <hardware/sensor/Touch.hpp>
#include <hardware/Bluetooth.hpp>

class NotificationService : public Service
{
    static constexpr const char* SERVICE_UUID =
        "25fb6509-75fc-4686-bbec-163d5c05ed8a";

    static constexpr const char* SERVER_NOTIFIY_UUID =
        "88c7bb0c-eed8-4964-9034-cf8f70e82158";

    BLEService* service = nullptr;

    // @brief Output Characteristic - Server to Client
    BLECharacteristic* OUT;

    void execute() override;

    std::string getName() override;

  public:
    NotificationService();
};