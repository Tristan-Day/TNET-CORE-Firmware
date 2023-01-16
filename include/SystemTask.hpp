#pragma once

#include <Arduino.h>

class SystemTask
{
    protected:
        TaskHandle_t taskHandle;
        SemaphoreHandle_t taskSemaphore;

    public:

        void suspend();

        void resume();
};