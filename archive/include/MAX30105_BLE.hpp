#include <Arduino.h>
#include <BLEUtils.h>

#include <MAX30105.h>
#include <heartRate.h>

#define MAX30105_BLE_VERBOSE

#define MAX30105_HR_SERVICE_TASK_STACK_DEPTH 5000
#define MAX30105_HR_INTERVAL 1
#define MAX30105_HR_BUFFER_SIZE 4

#define MAX30105_TEMPERATURE_SERVICE_UUID "cc4e954a-e598-4408-98a4-b854d38db30a"

#define MAX30105_BLE_SERVICE_TASK_STACK_DEPTH 5000
#define MAX30105_BLE_SAMPLE_TIME 15000
#define MAX30105_BLE_IR_THRESHOLD 5000

namespace MAX30105_HR
{
    extern MAX30105 *pParticleSensor;
    extern TaskHandle_t taskHandle;

    extern uint8_t buffer[MAX30105_HR_BUFFER_SIZE];
    extern uint8_t bufferCounter;
    extern uint32_t lastPulse;

    extern uint8_t average;

    void serviceTask(void *pParameters);

    void createService(MAX30105 *pParticleSensor);
}

namespace MAX30105_BLE
{
    extern MAX30105 particleSensor;
    extern TaskHandle_t taskHandle;

    extern BLEService *pHeartRateService;

    extern BLECharacteristic heartRateCharacteristic;
    extern BLEDescriptor heartRateDescriptor;

    extern BLECharacteristic sensorLocationCharacteristic;
    extern BLEDescriptor sensorLocationDescriptor;

    extern BLEService *pBodyTemperatureService;
    extern BLECharacteristic bodyTemperatureCharacteristic;
    extern BLEDescriptor bodyTemperatureDescriptor;

    extern bool connectionState;
    extern uint32_t sampleInterval;

    extern const uint8_t sensorLocation;

    void serviceTask(void *pParameters);

    void createService(BLEServer *pServer, uint32_t sampleInterval);

    void startService();

    void stopService();
}