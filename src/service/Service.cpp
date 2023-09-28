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
    Serial.println("[WARN] Task has no task implementation");
}

void Service::create(const char* name, uint32_t stack, uint8_t priority)
{
    taskSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(taskSemaphore);

    this->interval = DEFAULT_INTERVAL;

    xTaskCreate(task, name, stack, this, priority, &taskHandle);
}

void Service::create(const char* name, uint32_t stack, uint8_t priority, uint32_t interval)
{
    taskSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(taskSemaphore);

    this->interval = interval;

    xTaskCreate(task, name, stack, this, priority, &taskHandle);
}

void Service::start()
{
    xSemaphoreGive(taskSemaphore);
}

void Service::suspend()
{
    xSemaphoreTake(taskSemaphore, portMAX_DELAY);
}
