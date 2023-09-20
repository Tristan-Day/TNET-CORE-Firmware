#pragma once

#include <Arduino.h>

using namespace std;

class Service
{
  private:
    static constexpr long DEFAULT_INTERVAL = 30000;
    static constexpr long DEFAULT_STACK_DEPTH = 3000;

    TaskHandle_t taskHandle;
    SemaphoreHandle_t taskSemaphore;

    uint16_t interval = DEFAULT_INTERVAL;

    static void task(void* parameters);

  protected:
    virtual void execute();

    virtual uint32_t getStackDepth();

  public:
    Service();

    virtual string getName();

    void setInterval(uint32_t interval);

    void start();

    void suspend(TickType_t blockTime);

    void resume();
};