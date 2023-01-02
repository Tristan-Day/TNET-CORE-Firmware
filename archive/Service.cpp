#include <Service.hpp>

void Service::resume()
{
    xSemaphoreGive(taskSemaphore);
    vTaskResume(taskHandle);
}

void Service::suspend()
{
    xSemaphoreTake(taskSemaphore, portMAX_DELAY);
    vTaskSuspend(taskSemaphore);
}