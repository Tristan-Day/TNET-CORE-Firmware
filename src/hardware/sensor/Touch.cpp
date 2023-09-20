#include <hardware/sensor/Touch.hpp>

Touch* Touch::instance = nullptr;

Touch::Touch()
{
    lastEvent = UINT32_MAX;

    interruptEvent = xEventGroupCreate();
    touchEvent = xEventGroupCreate();

    setInterval(0);
    touchAttachInterrupt(INPUT_PIN, eventInterrupt, THRESHOLD);
}

void Touch::eventInterrupt()
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

    while (true)
    {
        xEventGroupWaitBits(self->interruptEvent, (1 << 0), true, false, UINT32_MAX);

        uint32_t startpoint = millis();

        while (touchInterruptGetLastStatus(INPUT_PIN))
        {
            delay(5);
        }

        uint32_t duration = millis() - startpoint;

        EventBits_t touchEvent = TouchEvent::NONE;

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

        if (touchEvent != TouchEvent::NONE)
        {
            xEventGroupSetBits(self->touchEvent, touchEvent);
            xEventGroupClearBits(self->touchEvent, touchEvent);

            delay(5000);
        }
    }
}

std::string Touch::getName()
{
    return "Touch Sense Service";
}

void Touch::init()
{
    if (instance == nullptr)
    {
        instance = new Touch();
        instance->start();
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

// #ifdef DEBUG
//     Serial.println("[------ TOUCH EVENT ------]");
//     Serial.printf("Event Duration: %ims\n", duration);
//     Serial.printf("Last Event: %ims\n", millis() - self->lastEvent);
//     Serial.printf("Event Type: %i\n", touchEvent);
//     Serial.println("[---- END TOUCH EVENT ----]");
// #endif