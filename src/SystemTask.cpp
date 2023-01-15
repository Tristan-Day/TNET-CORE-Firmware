#include <SystemTask.hpp>

void SystemTask::suspend()
{
    if (not suspended)
    {
        xSemaphoreTake(taskSemaphore, portMAX_DELAY);
    }
}

void SystemTask::resume()
{
    if (suspended)
    {
        xSemaphoreGive(taskSemaphore);
    }
}