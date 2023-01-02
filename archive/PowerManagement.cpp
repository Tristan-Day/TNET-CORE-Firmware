#include <PowerManagement.hpp>

namespace PowerManagementService
{

    BLEService *pService;

    BLECharacteristic powerModeCharacteristic(
        POWER_MANAGEMENT_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);
    BLEDescriptor powerModeDescriptor(BLEUUID((uint16_t)0x2901));

    uint8_t currentPowerProfile = 0;

}

void PowerManagementService::serviceTask(void *pParameters)
{
    powerModeCharacteristic.setValue(&currentPowerProfile, 1);
    while (true)
    {
        if (*powerModeCharacteristic.getData() != currentPowerProfile)
        {
            currentPowerProfile = *powerModeCharacteristic.getData();
            switch (currentPowerProfile)
            {
            case 0:
                PowerProfiles::active();
                break;
            case 1:
                PowerProfiles::optimised();
                break;
            case 2:
                PowerProfiles::powerSaving();
                break;
            case 3:
                PowerProfiles::ultaPowerSaving();
                break;
            default:
                break;
            }
        }
    }
}

void PowerManagementService::createService(BLEServer *pServer, uint16_t sampleInterval)
{
    pService = pServer->createService(POWER_PROFILE_SERVICE_UUID);
    pService->addCharacteristic(&powerModeCharacteristic);
    powerModeDescriptor.setValue("Device Power Profile");
    powerModeCharacteristic.addDescriptor(&powerModeDescriptor);

    pService->start();
    xTaskCreate(PowerManagementService::serviceTask,
                "Power Management Service",
                POWER_PROFILE_SERVICE_TASK_STACK_DEPTH,
                NULL,
                tskIDLE_PRIORITY,
                NULL);
}

void PowerProfiles::active()
{
    // Heart Rate Sampling Interval Set to 0 minutes
    // UV Sensing Sampling Interval Set to 0.5 minutes
}

void PowerProfiles::optimised()
{
    // Heart Rate Sampling Interval Set to 2 minutes
    // Enviromental Sampling Interval Set to 2 minutes
    // UV Sensing Sampling Interval Set to 1 minutes
}

void PowerProfiles::powerSaving()
{
    // Heart Rate Sampling Interval Set to 5 minutes
    // Enviromental Sampling Interval Set to 10 minutes
    // UV Sensing Sampling Interval Set to 2 Minutes
}

void PowerProfiles::ultaPowerSaving()
{
    // Heart Rate Samping Disabled
    // Enviromental Sampling Interval Set to 15 Minutes
    // UV Sensing Sampling Interval Set to 5 Minutes
}