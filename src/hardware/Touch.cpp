#include <hardware/Haptics.hpp>
#include <hardware/Touch.hpp>

EventGroupHandle_t TouchSense::eventInterruptGroup = xEventGroupCreate();

EventGroupHandle_t TouchSense::touchEventGroup = xEventGroupCreate();
TaskHandle_t TouchSense::taskHandle = NULL;

void TouchSense::interrupt()
{
    static BaseType_t xHigherPriorityTaskWoken;
    xEventGroupSetBitsFromISR(
        eventInterruptGroup, (1 << 0), &xHigherPriorityTaskWoken);
}

void TouchSense::eventTimer(void *)
{
    unsigned long startTime = 0;

    while (true)
    {
        xEventGroupWaitBits(
            eventInterruptGroup, (1 << 0), pdFALSE, pdFALSE, portMAX_DELAY);
        startTime = millis();

        delay(10);

        // Time the event
        while (touchRead(INPUT_PIN) < CAPACITANCE_THRESHOLD)
        {
            delay(10);
        }

        // Process the event
        eventProcessor(TouchEvent { millis(), millis() - startTime });

        // Reset the event interrupt group
        const EventBits_t xClearBits = (1 << 0);
        xEventGroupClearBits(eventInterruptGroup, xClearBits);
    }
}

void TouchSense::eventProcessor(TouchEvent event)
{
    static TouchEvent lastEvent;

#ifdef DEBUG
    Serial.println("[------ TOUCH EVENT ------]");
    Serial.printf("Event Timestamp: %i\n", event.timestamp);
    Serial.printf("Event Duration: %i\n", event.duration);
    Serial.println("[---- END TOUCH EVENT ----]");
#endif

    EventBits_t xBitsToSet = TouchEvent::NONE;
    unsigned long lastEventInterval = event.timestamp - lastEvent.timestamp;

    if (event.duration > LONG_HOLD_MIN and event.duration < LONG_HOLD_MAX)
    {
        xBitsToSet = TouchEvent::LONG_HOLD;
    }
    else if (lastEventInterval < DOUBLE_TAP_MAX)
    {
        xBitsToSet = TouchEvent::DOUBLE_TAP;
    }

    // Set and clear the touch event group
    if (xBitsToSet != TouchEvent::NONE)
    {
        xEventGroupSetBits(touchEventGroup, xBitsToSet);
        Haptics::vibrate(VibrationEffect::TICK);
        delay(1000);
        xEventGroupClearBits(touchEventGroup, xBitsToSet);

        lastEvent = TouchEvent();
    }
    else
    {
        lastEvent = event;
    }
}

void TouchSense::init()
{
    touchAttachInterrupt(INPUT_PIN, interrupt, CAPACITANCE_THRESHOLD);

    xTaskCreate(eventTimer, "Touch Sense Task", TASK_STACK_DEPTH, NULL,
        tskIDLE_PRIORITY, &taskHandle);
}