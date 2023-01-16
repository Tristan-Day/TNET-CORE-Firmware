#include <SystemTask.hpp>

void SystemTask::suspend()
{
    xSemaphoreTake(taskSemaphore, portMAX_DELAY);
}

void SystemTask::resume()
{
    xSemaphoreGive(taskSemaphore);
}