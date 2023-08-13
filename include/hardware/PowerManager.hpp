#pragma once

#include <hardware/Bluetooth.hpp>
#include <hardware/sensor/GNSS.hpp>

#include <service/Service.hpp>
#include <service/Console.hpp>

#include <Preferences.h>

struct PowerProfile
{
    static void optimised();

    static void powerSaving();

    static void ultraPowerSaving();

    enum
    {
        OPTIMISED,
        POWER_SAVING,
        ULTRA_POWER_SAVING
    };
};

class PowerManager
{
  private:
    static PowerManager* instance;

    Preferences preferences;

    PowerManager();

    static void eventHandler(void*);

    void setProfile(uint8_t profile, bool overwrite);

  public:
    static void init();

    static PowerManager* get();

    PowerManager(PowerManager& other) = delete;

    void operator=(const PowerManager&) = delete;

    void setProfile(uint8_t profile);

    uint8_t getProfile();
};