#include <Arduino.h>
#include <BLEUtils.h>

#include <Adafruit_BME280.h>

#define BME280_BLE_VERBOSE

#define _BME280_ADDRESS 0x76
#define BME280_SERVICE_TASK_STACK_DEPTH 5000
#define BME280_SEA_LEVEL_PRESSURE 1013.25

namespace BME280_BLE
{
    extern Adafruit_BME280 BME280;
    
    extern BLEService *pService;
    extern TaskHandle_t taskHandle;

    extern BLECharacteristic temperatureCharacteristic;
    extern BLEDescriptor temperatureDescriptor;

    extern BLECharacteristic pressureCharacteristic;
    extern BLEDescriptor pressureDescriptor;

    extern BLECharacteristic altitudeCharacteristic;
    extern BLEDescriptor altitudeDescriptor;

    extern BLECharacteristic humidityCharacteristic;
    extern BLEDescriptor humidityDescriptor;

    extern bool connectionState;
    extern uint16_t sampleInterval;

    void serviceTask(void *pParameters);

    void createService(BLEServer *pServer, uint16_t sampleInterval);

    void startService();

    void stopService();
}