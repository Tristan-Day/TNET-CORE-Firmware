#pragma once

#include <Arduino.h>

class SystemTask
{
    private:
        bool suspended;

    protected:
        TaskHandle_t taskHandle;
        SemaphoreHandle_t taskSemaphore;

    public:

        void suspend();

        void resume();
};