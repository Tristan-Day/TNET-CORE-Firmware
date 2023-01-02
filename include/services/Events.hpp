#include <Arduino.h>
#include <BLEUtils.h>
#include <Preferences.h>

#include <Devices.hpp>

#define EVENTS_PREFS_NAMESPACE "NOTIFICATIONS"

namespace EventService
{
  extern BLEService *pService;

  class Callbacks : public BLECharacteristicCallbacks
  {
    void onWrite(BLECharacteristic *pCharacteristic);
  };

  void createService(BLEServer *pServer);
} // namespace EventService
