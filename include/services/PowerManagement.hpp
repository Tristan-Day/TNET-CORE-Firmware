#include <Arduino.h>
#include <BLEUtils.h>
#include <Preferences.h>

#include <Devices.hpp>

#include <services/HeartRate.hpp>

#define POWER_MANAGEMENT_PREFS_NAMESPACE "PWR_MANGMNT"
#define POWER_MANAGEMENT_TASK_STACK_DEPTH 2000
#define POWER_MANAGEMENT_SAMPLE_INTERVAL 10000

#define POWER_MANAGEMENT_MODE_UUID "b9c1209b-4c6b-4226-ad92-18b1f1bea61f"

namespace PowerManagementService
{
    extern Preferences preferences;

    extern BLEService *pService;
    extern TaskHandle_t taskHandle;

    extern BLECharacteristic batteryVoltageCharacteristic;
    extern BLEDescriptor batteryVoltageDescriptor;

    extern BLECharacteristic batteryCharageCharacteristic;
    extern BLEDescriptor batteryCharageDescriptor;

    extern BLECharacteristic powerProfileCharacteristic;
    extern BLEDescriptor powerProfileDescriptor;

    class Callbacks : public BLECharacteristicCallbacks
    {
        void onWrite(BLECharacteristic* pCharacteristic);
    };

    void createService(BLEServer *pServer);

    void serviceTask(void*);

    void setPowerProfile(uint8_t profile);
}

namespace PowerProfiles
{
    typedef enum {
		ACTIVE,
		OPTIMISED,
		POWER_SAVING,
		ULTA_POWER_SAVING
	}PowerProfile;

    void active();

    void optimised();

    void powerSaving();

    void ultaPowerSaving();
}