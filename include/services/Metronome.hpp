#include <Arduino.h>
#include <BLEUtils.h>
#include <Preferences.h>
#include <freertos/semphr.h>

#include <Devices.hpp>

#define METRONOME_PREFS_NAMESPACE "METRONOME"
#define METRONOME_TASK_STACK_DEPTH 2000
#define METRONOME_BEAT_DURATION 200

#define METRONOME_SERVICE_UUID "429b5aa8-d015-4705-99db-a51c49fc84b6"
#define METRONOME_TEMPO_UUID "726eb1ec-e713-400a-a7d3-b38a94d7095c"

namespace Metronome
{
  extern Preferences preferences;

  extern BLEService *pService;

  extern TaskHandle_t taskHandle;
  extern SemaphoreHandle_t semaphoreHandle;

  extern BLECharacteristic stateCharacteristic;
  extern BLEDescriptor stateDescriptor;

  extern BLECharacteristic tempoCharacteristic;
  extern BLEDescriptor tempoDescriptor;

  extern long beatInterval;

  class Callbacks : public BLECharacteristicCallbacks
  {
    void onWrite(BLECharacteristic *pCharacteristic);
  };

  void createService(BLEServer *pServer);

  void serviceTask(void *);
} // namespace Metronome