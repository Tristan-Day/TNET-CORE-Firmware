#include <hardware/sensor/Touch.hpp>

Touch* Touch::instance = nullptr;

Touch::Touch()
{
    lastEvent = UINT32_MAX;

    interruptEvent = xEventGroupCreate();
    touchEvent = xEventGroupCreate();

    touchAttachInterrupt(INPUT_PIN, interrupt, THRESHOLD);

    auto lambda = [&]()
    {
        xEventGroupWaitBits(interruptEvent, (1 << 0), true, false, portMAX_DELAY);

        uint32_t startpoint = millis();
        while (touchInterruptGetLastStatus(INPUT_PIN))
        {
            delay(5);
        }
        uint32_t duration = millis() - startpoint;
        
        EventBits_t event = 0;

        if (duration > LONG_PRESS)
        {
            event = TouchEvent::LONG_PRESS;
            Haptics::get()->play(VibrationEffect::CLICK);
        }
        else if (duration > SHORT_PRESS && duration < LONG_PRESS)
        {
            uint32_t interval = millis() - lastEvent;

            if (interval < 2000)
            {
                event = TouchEvent::DOUBLE_PRESS;
                Haptics::get()->play(VibrationEffect::DOUBLE_CLICK);
            }

            lastEvent = millis();
        }

        if (event != _NULL)
        {
            xEventGroupSetBits(touchEvent, event);
            xEventGroupClearBits(touchEvent, event);
        }
    };

    eventProcessor = new Microservice(lambda, 15);
    eventProcessor->start("TEP", 2000, 10);
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
