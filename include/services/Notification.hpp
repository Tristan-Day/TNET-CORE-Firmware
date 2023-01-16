#pragma once

#include <BLE2902.h>
#include <BLEUtils.h>

#include <SystemTask.hpp>
#include <hardware/Haptics.hpp>
#include <hardware/Touch.hpp>

class NotificationService : public BLECharacteristicCallbacks
{
private:
    static const std::string SERVICE_UUID;
    static const std::string SERVER_NOTIFICATION_UUID;
    static const std::string CLIENT_NOTIFICATION_UUID;

    static const uint16_t TASK_STACK_DEPTH = 2000;

    static NotificationService *pInstance;

    BLEService *pService;

    // Device -> Client
    BLECharacteristic serverNotificationCharacteristic;
    BLEDescriptor serverNotificationDescriptor;

    // Client -> Device
    BLECharacteristic clientNotificationCharacteristic;
    BLEDescriptor clientNotificationDescriptor;

    NotificationService()
        : serverNotificationCharacteristic(
            SERVER_NOTIFICATION_UUID, BLECharacteristic::PROPERTY_NOTIFY),
          serverNotificationDescriptor(BLEUUID((uint16_t)0x2901)),

          clientNotificationCharacteristic(
              CLIENT_NOTIFICATION_UUID, BLECharacteristic::PROPERTY_WRITE),
          clientNotificationDescriptor(BLEUUID((uint16_t)0x2901)) {};

    void onWrite(BLECharacteristic *pCharacteristic);

    static void touchEventHandler(void *);

public:
    NotificationService(const NotificationService &obj) = delete;

    static NotificationService *get();

    void init(BLEServer *pServer);
};