#pragma once

#include <BLEUtils.h>
#include <Preferences.h>

#include <SystemTask.hpp>
#include <hardware/Haptics.hpp>

#define METRONOME_SERVICE_UUID "429b5aa8-d015-4705-99db-a51c49fc84b6"
#define METRONOME_TEMPO_UUID "726eb1ec-e713-400a-a7d3-b38a94d7095c"

class Metronome : public SystemTask, BLECharacteristicCallbacks
{
private:
    static const uint16_t TASK_STACK_DEPTH = 2000;
    static const uint16_t BEAT_DURATION = 200;

    static Metronome *pInstance;

    Preferences preferences;
    BLEService *pService;

    BLECharacteristic stateCharacteristic;
    BLEDescriptor stateDescriptor;

    BLECharacteristic tempoCharacteristic;
    BLEDescriptor tempoDescriptor;

    unsigned long beatInterval;

    Metronome()
        : stateCharacteristic(BLEUUID((uint16_t)0x2AE2),
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE),
          stateDescriptor(BLEUUID((uint16_t)0x2901)),

          tempoCharacteristic(METRONOME_TEMPO_UUID,
              BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE),
          tempoDescriptor(BLEUUID((uint16_t)0x2901)) {};

    void onWrite(BLECharacteristic *pCharacteristic);

    static void task(void *);

public:
    Metronome(const Metronome &obj) = delete;

    static Metronome *get();

    void init(BLEServer *pServer);

    void setTempo(uint8_t tempo);
};