#include <services/PowerManagement.hpp>

#define DEBUG

namespace PowerManagementService
{
    Preferences preferences;

    BLEService *pService;
    TaskHandle_t taskHandle;

    BLECharacteristic batteryVoltageCharacteristic(
        BLEUUID((uint16_t)0x2B18),
        BLECharacteristic::PROPERTY_READ);
    BLEDescriptor batteryVoltageDescriptor(BLEUUID((uint16_t)0x2901));

    BLECharacteristic batteryCharageCharacteristic(
        BLEUUID((uint16_t)0x2A19),
        BLECharacteristic::PROPERTY_READ);
    BLEDescriptor batteryCharageDescriptor(BLEUUID((uint16_t)0x2901));

    BLECharacteristic powerProfileCharacteristic(
        POWER_MANAGEMENT_MODE_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE);
    BLEDescriptor powerProfileDescriptor(BLEUUID((uint16_t)0x2901));
}

void PowerManagementService::Callbacks::onWrite(BLECharacteristic* pCharacteristic)
{
#ifdef DEBUG
    Serial.println("[INFO] Write Callback for Power Mode Chraracteristic");
#endif

    setPowerProfile(*powerProfileCharacteristic.getData());
}

void PowerManagementService::createService(BLEServer *pServer)
{
    preferences.begin("PWR_MANGMNT");

    pService = pServer->createService(BLEUUID((uint16_t)0x180F));

    pService->addCharacteristic(&batteryVoltageCharacteristic);
    batteryVoltageDescriptor.setValue("Battery Voltage");
    batteryVoltageCharacteristic.addDescriptor(&batteryVoltageDescriptor);

    pService->addCharacteristic(&batteryCharageCharacteristic);
    batteryCharageDescriptor.setValue("Battery Level");
    batteryCharageCharacteristic.addDescriptor(&batteryCharageDescriptor);

    pService->addCharacteristic(&powerProfileCharacteristic);
    powerProfileDescriptor.setValue("Power Profile: 0-3");
    powerProfileCharacteristic.addDescriptor(&powerProfileDescriptor);
    powerProfileCharacteristic.setCallbacks(new PowerManagementService::Callbacks);

    pService->start();

    uint8_t powerProfile = preferences.getUChar("PWR_PROFILE", PowerProfiles::OPTIMISED);
    setPowerProfile(powerProfile);

    xTaskCreate(serviceTask,
                "Power Management Service",
                POWER_MANAGEMENT_TASK_STACK_DEPTH,
                NULL,
                tskIDLE_PRIORITY,
                &taskHandle);
}

void PowerManagementService::serviceTask(void*)
{
    while (true)
    {
        uint16_t batteryVoltage = (Battery::getVoltage()) * 100;
        batteryVoltageCharacteristic.setValue((uint8_t *)&batteryVoltage, 2);

        uint8_t batteryCharge = Battery::getPercent();
        batteryCharageCharacteristic.setValue((uint8_t *)&batteryCharge, 1);

        delay(POWER_MANAGEMENT_SAMPLE_INTERVAL); 
    }
}

void PowerManagementService::setPowerProfile(uint8_t powerProfile)
{
    switch (powerProfile)
    {
    case PowerProfiles::ACTIVE:
        PowerProfiles::active();
        break;

    case PowerProfiles::OPTIMISED:
        PowerProfiles::optimised();
        break;

    case PowerProfiles::POWER_SAVING:
        PowerProfiles::powerSaving();
        break;

    case PowerProfiles::ULTA_POWER_SAVING:
        PowerProfiles::ultaPowerSaving();
        break;
    
    default:
        powerProfile = PowerProfiles::OPTIMISED;
        break;
    }
    powerProfileCharacteristic.setValue((uint8_t *)&powerProfile, PowerProfiles::OPTIMISED);
    preferences.putUChar("PWR_PROFILE", powerProfile);
}

void PowerProfiles::active()
{
    setCpuFrequencyMhz(240);

    MAX30105::sampleInterval = 5000;
    BME280::sampleInterval = 60000;
    GY8511::sampleInterval = 30000;

    MAX30105::resume();

#ifdef DEBUG
  Serial.println("[INFO] Power Management Profile 'Active' Applied");
#endif
}

void PowerProfiles::optimised()
{

    setCpuFrequencyMhz(160);

    MAX30105::sampleInterval = 120000;  // Every 2 Minutes
    BME280::sampleInterval = 60000;     // Every Minute
    GY8511::sampleInterval = 60000;     // Every Minute

    MAX30105::resume();

#ifdef DEBUG
  Serial.println("[INFO] Power Management Profile 'Optimised' Applied");
#endif
}

void PowerProfiles::powerSaving()
{
    setCpuFrequencyMhz(80);

    MAX30105::sampleInterval = 300000;  // Every 5 Minutes
    BME280::sampleInterval = 600000;    // Every 10 Minutes
    GY8511::sampleInterval = 120000;    // Every 2 Minutes

    MAX30105::resume();

#ifdef DEBUG
  Serial.println("[INFO] Power Management Profile 'Power Saving' Applied");
#endif
}

void PowerProfiles::ultaPowerSaving()
{
    setCpuFrequencyMhz(80);

    BME280::sampleInterval = 900000;    // Every 15 Minutes
    GY8511::sampleInterval = 300000;    // Every 5 Minutes

    MAX30105::pause();

#ifdef DEBUG
  Serial.println("[INFO] Power Management Profile 'Ultra Power Saving' Applied");
#endif
}

