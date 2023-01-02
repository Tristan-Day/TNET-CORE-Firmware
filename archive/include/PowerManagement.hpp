#include <Arduino.h>
#include <BLEUtils.h>

#define POWER_PROFILE_SERVICE_TASK_STACK_DEPTH 5000

#define POWER_PROFILE_ATTRIBUTE_UUID "e8d12bcf-7271-4a8b-abdb-70c5f2af0521"
#define POWER_PROFILE_SERVICE_UUID "cad13353-7585-49b5-9181-88c8fcc180cd"

namespace PowerManagementService
{

    extern BLEService *pService;

    extern BLECharacteristic powerModeCharacteristic;
    extern BLEDescriptor powerModeDescriptor;

    extern uint8_t currentPowerProfile;

    void serviceTask(void *pParameters);

    void createService(BLEServer *pServer, uint8_t sampleInterval);
}

namespace PowerProfiles
{
    void active();

    void optimised();

    void powerSaving();

    void ultaPowerSaving();
}