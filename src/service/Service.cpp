#include <service/Service.hpp>

void Service::task(void* parameters)
{
    Service* service = static_cast<Service*>(parameters);

    while (true)
    {
        xSemaphoreTake(service->taskSemaphore, portMAX_DELAY);

        service->execute();
  
        xSemaphoreGive(service->taskSemaphore);
        vTaskDelay(pdMS_TO_TICKS(service->interval));
    }
}

void Service::execute()
{
    string message = "[WARN] Task '" + getName() + "' has no task implementation";
    Serial.println(message.c_str());
}

uint32_t Service::getStackDepth()
{
    return DEFAULT_STACK_DEPTH;
}

void Service::setFailed(bool state)
{
    failed = state;

#ifdef DEBUG
    if (state)
    {
        string message = "[ERROR] Task '" + getName() + "' failed";
        Serial.println(message.c_str());
    }
#endif
}

Service::Service()
{
    taskSemaphore = xSemaphoreCreateBinary();
}

string Service::getName()
{
    return "Untitled Service";
}

void Service::setInterval(uint32_t interval)
{
    this->interval = interval;
}

bool Service::hasFailed()
{
    return failed;
}

void Service::start()
{
    if (taskHandle != NULL)
    {
        taskSemaphore = xSemaphoreCreateBinary();
        xSemaphoreGive(taskSemaphore);

        xTaskCreate(task, getName().c_str(), getStackDepth(), this, tskIDLE_PRIORITY, &taskHandle);
    }
}

void Service::suspend(TickType_t blockTime)
{
    xSemaphoreTake(taskSemaphore, blockTime);
}

void Service::resume()
{
    xSemaphoreGive(taskSemaphore);
}