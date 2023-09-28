#pragma once

#include <Arduino.h>

using namespace std;

static constexpr long DEFAULT_INTERVAL = 30000;
static constexpr long DEFAULT_STACK_DEPTH = 3000;

class Microservice
{
  private:
    TaskHandle_t taskHandle;
    SemaphoreHandle_t taskSemaphore;

    function<void()> operation;
    uint32_t interval;

    static void task(void* parameters);

  protected:
    virtual void execute();

  public:

    Microservice(function<void()> operation, uint32_t interval);

    void start(const char* name, uint32_t stack, uint8_t priority);

    void suspend();

    void resume();
};