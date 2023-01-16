#pragma once

#include <BLEUtils.h>
#include <Preferences.h>

#include <SystemTask.hpp>
#include <hardware/Haptics.hpp>

class MetronomeService : public SystemTask, BLECharacteristicCallbacks
{
private:
    static const std::string SERVICE_UUID;
    static const std::string TEMPO_UUID;

    static const uint16_t TASK_STACK_DEPTH = 2000;
    static const uint16_t BEAT_DURATION = 200;

    static MetronomeService *pInstance;

    Preferences preferences;
    BLEService *pService;

    BLECharacteristic stateCharacteristic;
    BLEDescriptor stateDescriptor;

    BLECharacteristic tempoCharacteristic;
    BLEDescriptor tempoDescriptor;

    unsigned long beatInterval;

    MetronomeService()
        : stateCharacteristic(BLEUUID((uint16_t)0x2AE2),
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE),
          stateDescriptor(BLEUUID((uint16_t)0x2901)),

          tempoCharacteristic(TEMPO_UUID,
              BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE),
          tempoDescriptor(BLEUUID((uint16_t)0x2901)) {};

    void onWrite(BLECharacteristic *pCharacteristic);

    static void task(void *);

public:
    MetronomeService(const MetronomeService &obj) = delete;

    static MetronomeService *get();

    void init(BLEServer *pServer);

    void setTempo(uint8_t tempo);
};