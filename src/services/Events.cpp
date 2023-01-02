#include <services/Events.hpp>

namespace EventService
{
    BLEService *pService;

}

void EventService::Callbacks::onWrite(BLECharacteristic *pCharacteristic)
{

}

void EventService::createService(BLEServer *pServer)
{
    
}