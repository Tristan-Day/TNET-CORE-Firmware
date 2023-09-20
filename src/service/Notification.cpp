#include <service/Notification.hpp>

NotificationService::NotificationService()
{
    service = Bluetooth::get()->server->createService(SERVICE_UUID);

    OUT = service->createCharacteristic(
        BLEUUID(SERVER_NOTIFIY_UUID), NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

    setInterval(0);
    service->start();
}

void NotificationService::execute()
{
    EventBits_t events = TouchEvent::LONG_PRESS | TouchEvent::DOUBLE_PRESS;
    EventBits_t event = xEventGroupWaitBits(Touch::get()->touchEvent, events, false, false, UINT32_MAX);

#ifdef DEBUG
    Serial.println("[INFO] Writing to Notification Characteristic");
#endif

    OUT->setValue(event);
    OUT->notify();
}

std::string NotificationService::getName()
{
    return "Notification Service";
}
