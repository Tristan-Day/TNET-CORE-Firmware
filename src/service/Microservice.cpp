#include <service/Microservice.hpp>

void Microservice::task(void* parameters)
{
    Microservice* service = static_cast<Microservice*>(parameters);

    while (true)
    {
        xSemaphoreTake(service->taskSemaphore, portMAX_DELAY);

        service->operation();

        xSemaphoreGive(service->taskSemaphore);
        vTaskDelay(pdMS_TO_TICKS(service->interval));
    }
}

void Microservice::execute()
{
    Serial.println("[WARN] Task has no task implementation");
}

Microservice::Microservice(function<void()> operation, uint32_t interval)
{
    this->operation = operation;
    this->interval = interval;
}

Microservice::~Microservice()
{
    vTaskDelete(taskHandle);
    vSemaphoreDelete(taskSemaphore);
}

void Microservice::start(const char* name, uint32_t stack, uint8_t priority)
{
    taskSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(taskSemaphore);

    xTaskCreate(task, name, stack, this, priority, &taskHandle);
}

void Microservice::suspend()
{
    xSemaphoreTake(taskSemaphore, portMAX_DELAY);
}

void Microservice::resume()
{
    xSemaphoreGive(taskSemaphore);
}