#include <hardware/sensor/Touch.hpp>

Touch* Touch::instance = nullptr;

Touch::Touch()
{
    lastEvent = UINT32_MAX;

    interruptEvent = xEventGroupCreate();
    touchEvent = xEventGroupCreate();

    touchAttachInterrupt(INPUT_PIN, interrupt, THRESHOLD);

    this->create("Touch Service", 2000, 10, 15);
}

void Touch::interrupt()
{
    Touch* self = Touch::get();

#ifdef DEBUG
    Serial.println("[INFO] Touch Event Detected");
#endif

    static BaseType_t taskUnblocked;
    xEventGroupSetBitsFromISR(self->interruptEvent, (1 << 0), &taskUnblocked);
}

void Touch::execute()
{
    Touch* self = Touch::get();

    xEventGroupWaitBits(self->interruptEvent, (1 << 0), true, false, UINT32_MAX);

    // Measure the event
    uint32_t startpoint = millis();

    while (touchInterruptGetLastStatus(INPUT_PIN))
    {
        delay(5);
    }

    uint32_t duration = millis() - startpoint;

    // Process the event
    EventBits_t touchEvent = _NULL;

    if (duration > LONG_PRESS)
    {
        touchEvent = TouchEvent::LONG_PRESS;
        Haptics::get()->play(VibrationEffect::CLICK);
    }

    else if (duration > SHORT_PRESS && duration < LONG_PRESS)
    {
        uint32_t interval = millis() - self->lastEvent;

        if (interval < MAX_INTERVAL)
        {
            touchEvent = TouchEvent::DOUBLE_PRESS;
            Haptics::get()->play(VibrationEffect::DOUBLE_CLICK);
        }

        self->lastEvent = millis();
    }

    if (touchEvent != _NULL)
    {
        xEventGroupSetBits(self->touchEvent, touchEvent);
        xEventGroupClearBits(self->touchEvent, touchEvent);

        delay(5000);
    }
}

void Touch::init()
{
    if (instance == nullptr)
    {
        instance = new Touch();
    }
}

Touch* Touch::get()
{
    if (instance == nullptr)
    {
        init();
    }
    return instance;
}
