#include <hardware/PowerManager.hpp>

void PowerProfile::optimised()
{
    setCpuFrequencyMhz(160);

    GNSS::get()->sensor->powerSaveMode(false, 1000);

#ifdef DEBUG
    Serial.println("[INFO] Optimised Profile Applied");
#endif
}

void PowerProfile::powerSaving()
{
    setCpuFrequencyMhz(80);

    GNSS::get()->sensor->powerSaveMode(true, 1000);

#ifdef DEBUG
    Serial.println("[INFO] Power Saving Profile Applied");
#endif
}

void PowerProfile::ultraPowerSaving()
{
    setCpuFrequencyMhz(80);

    GNSS::get()->sensor->powerSaveMode(true, 1000);

#ifdef DEBUG
    Serial.println("[INFO] Ultra Power Saving Profile Applied");
#endif
}

PowerManager* PowerManager::instance = nullptr;

PowerManager::PowerManager()
{
    preferences.begin("PWR MANAGER");
    setProfile(getProfile(), false);

    xTaskCreate(eventHandler, "Bluetooth Event Handler", DEFAULT_STACK_DEPTH, NULL,
                tskIDLE_PRIORITY + 1, NULL);
}

void PowerManager::eventHandler(void*)
{
    while (true)
    {
        xEventGroupWaitBits(Bluetooth::get()->bluetoothEventGroup, (1 << 0), pdFALSE,
                            pdFALSE, portMAX_DELAY);

        get()->setProfile(get()->preferences.getUChar("PROFILE", PowerProfile::OPTIMISED), false);

        xEventGroupWaitBits(Bluetooth::get()->bluetoothEventGroup, (1 << 1), pdFALSE,
                            pdFALSE, portMAX_DELAY);

        get()->setProfile(PowerProfile::ULTRA_POWER_SAVING, false);
    }
}

void PowerManager::setProfile(uint8_t profile, bool overwrite)
{
    switch (profile)
    {
    case PowerProfile::OPTIMISED:
        PowerProfile::optimised();
        break;

    case PowerProfile::POWER_SAVING:
        PowerProfile::powerSaving();
        break;

    case PowerProfile::ULTRA_POWER_SAVING:
        PowerProfile::ultraPowerSaving();
        break;

    default:
        return;
    }

    if (overwrite)
    {
        preferences.putUChar("PROFILE", profile);
    }
}

void PowerManager::init()
{
    if (instance == nullptr)
    {
        instance = new PowerManager();
    }
}

PowerManager* PowerManager::get()
{
    if (instance == nullptr)
    {
        init();
    }
    return instance;
}

void PowerManager::setProfile(uint8_t profile)
{
    setProfile(profile, true);
}

uint8_t PowerManager::getProfile()
{
    return preferences.getUChar("PROFILE", PowerProfile::OPTIMISED);
}
