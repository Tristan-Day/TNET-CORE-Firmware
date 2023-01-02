#include <Arduino.h>
#include <BLEUtils.h>

#define USER_DESCRIPTOR BLEUUID((uint16_t)0x2901)

class Service : public BLECharacteristicCallbacks
{
protected:
    BLEService *pService;

    TaskHandle_t taskHandle;
    SemaphoreHandle_t taskSemaphore;

public:
    void suspend();

    void resume();
};