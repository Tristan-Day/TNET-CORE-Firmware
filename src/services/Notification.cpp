#include <services/Notification.hpp>

NotificationService *NotificationService::pInstance;

const std::string NotificationService::SERVICE_UUID
    = "25fb6509-75fc-4686-bbec-163d5c05ed8a";

const std::string NotificationService::SERVER_NOTIFICATION_UUID
    = "88c7bb0c-eed8-4964-9034-cf8f70e82158";

const std::string NotificationService::CLIENT_NOTIFICATION_UUID
    = "d800afab-8ca3-48f4-90fa-3c65b2b1c7ae";

void NotificationService::onWrite(BLECharacteristic *pCharacteristic)
{
    VibrationEffect *effect = NULL;

    switch (*clientNotificationCharacteristic.getData())
    {
    case 1:
        effect = VibrationEffect::TICK;
        break;

    default:
        break;
    }

    if (effect != NULL)
    {
        Haptics::aquireLock();
        Haptics::vibrate(effect);
        Haptics::releaseLock();
    }
}

void NotificationService::touchEventHandler(void *)
{
    NotificationService *self = get();

    while (true)
    {
        uint8_t eventType = xEventGroupWaitBits(
            TouchSense::touchEventGroup, 0b11, pdTRUE, pdFALSE, portMAX_DELAY);

        self->serverNotificationCharacteristic.setValue((uint8_t *)&eventType, 1);
        self->serverNotificationCharacteristic.notify();
    }
}

NotificationService *NotificationService::get()
{
    if (pInstance == NULL)
    {
        pInstance = new NotificationService();
    }
    return pInstance;
}

void NotificationService::init(BLEServer *pServer)
{
    pService = pServer->createService(SERVICE_UUID);

    pService->addCharacteristic(&serverNotificationCharacteristic);
    serverNotificationDescriptor.setValue("Notifications from Device");
    serverNotificationCharacteristic.addDescriptor(&serverNotificationDescriptor);
    serverNotificationCharacteristic.addDescriptor(new BLE2902);

    pService->addCharacteristic(&clientNotificationCharacteristic);
    clientNotificationDescriptor.setValue("Notifications to Device");
    clientNotificationCharacteristic.addDescriptor(&clientNotificationDescriptor);
    clientNotificationCharacteristic.setCallbacks(this);

    pService->start();

    xTaskCreate(touchEventHandler, "Notification Touch Event Handler",
        TASK_STACK_DEPTH, NULL, tskIDLE_PRIORITY, NULL);
}