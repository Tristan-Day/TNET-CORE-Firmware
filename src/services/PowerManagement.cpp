#include <services/PowerManagement.hpp>

#define DEBUG

void PowerProfile::active()
{
    setCpuFrequencyMhz(240);

    MAX30105::get()->sampleInterval = 5000;
    BME280::get()->sampleInterval = 60000;
    GY8511::sampleInterval = 30000;

    MAX30105::get()->resume();

#ifdef DEBUG
    Serial.println("[INFO] Active Profile Applied");
#endif
}

void PowerProfile::optimised()
{
    setCpuFrequencyMhz(160);

    MAX30105::get()->sampleInterval = 120000;   // Every 2 Minutes
    BME280::get()->sampleInterval = 60000;      // Every Minute
    GY8511::sampleInterval = 60000;             // Every Minute

    MAX30105::get()->resume();

#ifdef DEBUG
    Serial.println("[INFO] Optimised Profile Applied");
#endif
}

void PowerProfile::powerSaving()
{
    setCpuFrequencyMhz(80);

    MAX30105::get()->sampleInterval = 300000;   // Every 5 Minutes
    BME280::get()->sampleInterval = 60000;      // Every 10 Minutes
    GY8511::sampleInterval = 120000;            // Every 2 Minutes

    MAX30105::get()->resume();

#ifdef DEBUG
    Serial.println("[INFO] Power Saving Profile Applied");
#endif
}

void PowerProfile::ultaPowerSaving()
{
    setCpuFrequencyMhz(80);

    BME280::get()->sampleInterval = 900000;     // Every 15 Minutes
    GY8511::sampleInterval = 300000;            // Every 5 Minutes

    MAX30105::get()->suspend();

#ifdef DEBUG
    Serial.println("[INFO] Ultra Power Saving Profile Applied");
#endif
}

PowerManagement *PowerManagement::pInstance;

void PowerManagement::onWrite(BLECharacteristic *pCharacteristic)
{
#ifdef DEBUG
    Serial.println("[INFO] Write Callback for Power Profile Chraracteristic");
#endif

    setProfile(*powerProfileCharacteristic.getData());
    preferences.putUChar("PROFILE", *powerProfileCharacteristic.getData());
}

// clang-format off
void PowerManagement::bluetoothEventHandler(void *)
{
    while (true)
    {
        xEventGroupWaitBits(Bluetooth::get()->serverEventGroup, (1 << 0), pdFALSE,
            pdFALSE, portMAX_DELAY);

        get()->setProfile(get()->preferences.getUChar("PROFILE", PowerProfile::OPTIMISED));

        xEventGroupWaitBits(Bluetooth::get()->serverEventGroup, (1 << 1), pdFALSE,
            pdFALSE, portMAX_DELAY);

        get()->setProfile(PowerProfile::ULTRA_POWER_SAVING);
    }
}
// clang-format on

void PowerManagement::batteryMonitorTask(void *)
{
    while (true)
    {
        uint8_t batteryCharge = Battery::getChargePercent();
        get()->batteryCharageCharacteristic.setValue((uint8_t *)&batteryCharge, 1);

        delay(Battery::sampleInterval);
    }
}

PowerManagement *PowerManagement::get()
{
    if (pInstance == NULL)
    {
        pInstance = new PowerManagement();
    }
    return pInstance;
}

void PowerManagement::init(BLEServer *pServer)
{
    preferences.begin("PWR MANAGEMENT");
    pService = pServer->createService(BLEUUID((uint16_t)0x180F));

    pService->addCharacteristic(&batteryCharageCharacteristic);
    batteryCharageCharacteristic.addDescriptor(new BLE2902);

    pService->addCharacteristic(&powerProfileCharacteristic);
    powerProfileDescriptor.setValue("Power Profile (0-3)");
    powerProfileCharacteristic.addDescriptor(&powerProfileDescriptor);
    powerProfileCharacteristic.setCallbacks(this);

    pService->start();

    setProfile(PowerProfile::ULTRA_POWER_SAVING);

    xTaskCreate(bluetoothEventHandler, "Bluetooth Event Handler", TASK_STACK_DEPTH,
        NULL, tskIDLE_PRIORITY + 1, NULL);

    xTaskCreate(batteryMonitorTask, "Battery Monitoring Service", TASK_STACK_DEPTH,
        NULL, tskIDLE_PRIORITY + 0, NULL);
}

void PowerManagement::setProfile(uint8_t profile)
{
    switch (profile)
    {
    case PowerProfile::ACTIVE:
        PowerProfile::active();
        break;

    case PowerProfile::OPTIMISED:
        PowerProfile::optimised();
        break;

    case PowerProfile::POWER_SAVING:
        PowerProfile::powerSaving();
        break;

    case PowerProfile::ULTRA_POWER_SAVING:
        PowerProfile::ultaPowerSaving();
        break;
    }

    powerProfileCharacteristic.setValue((uint8_t *)&profile, 1);
}