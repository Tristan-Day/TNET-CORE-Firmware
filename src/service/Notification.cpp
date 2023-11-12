#include <service/Notification.hpp>

NotificationService::NotificationService()
{
    service = Bluetooth::get()->server->createService(SERVICE_UUID);

    OUT = service->createCharacteristic(
        BLEUUID(SERVER_NOTIFIY_UUID), NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

    service->start();

    auto lambda = [&]()
    {
        EventBits_t events = TouchEvent::LONG_PRESS | TouchEvent::DOUBLE_PRESS;
        EventBits_t event = xEventGroupWaitBits(Touch::get()->touchEvent, events, false, false, portMAX_DELAY);

    #ifdef DEBUG
        Serial.println("[INFO] Writing to Notification Characteristic");
    #endif

        OUT->setValue(event);
        OUT->notify();
    };

    eventProcessor = new Microservice(lambda, 15);
    eventProcessor->start("HID", 3000, 5);
}

NotificationService::~NotificationService()
{
    delete eventProcessor;

    service->removeCharacteristic(OUT, true);
    Bluetooth::get()->server->removeService(service, true);
}
