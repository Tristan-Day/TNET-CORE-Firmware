#pragma once

#include <Arduino.h>

using namespace std;

constexpr long DEFAULT_INTERVAL = 30000;
constexpr long DEFAULT_STACK_DEPTH = 3000;

class Service
{
  private:
    TaskHandle_t taskHandle;
    SemaphoreHandle_t taskSemaphore;

    uint16_t interval = DEFAULT_INTERVAL;
    bool failed = false;

    static void task(void* parameters);

  protected:
    virtual void execute();

    virtual uint32_t getStackDepth();

    void setFailed(bool state);

  public:
    Service();

    virtual string getName();

    void setInterval(uint32_t interval);

    bool hasFailed();

    void start();

    void suspend(TickType_t blockTime);

    void resume();
};