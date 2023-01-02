#include <services/Metronome.hpp>

namespace Metronome
{
    Preferences preferences;

    BLEService *pService;

    TaskHandle_t taskHandle;
    SemaphoreHandle_t semaphoreHandle = NULL;

    BLECharacteristic stateCharacteristic(
        BLEUUID((uint16_t)0x2AE2),
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE);
    BLEDescriptor stateDescriptor(BLEUUID((uint16_t)0x2901));

    BLECharacteristic tempoCharacteristic(
        METRONOME_TEMPO_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE);
    BLEDescriptor tempoDescriptor(BLEUUID((uint16_t)0x2901));

    long beatInterval;
}

void Metronome::Callbacks::onWrite(BLECharacteristic *pCharacteristic)
{
    std::string characteristicUUID = pCharacteristic->getUUID().toString();

#ifdef DEBUG
    Serial.print("[INFO] Write Callback for Characteristic: ");
    Serial.println(characteristicUUID.c_str());
#endif

    if (characteristicUUID == BLEUUID((uint16_t)0x2AE2).toString())
    {
        if ((bool)*pCharacteristic->getData())
        {
            Haptics::mutex.lock();
            xSemaphoreGive(semaphoreHandle);
        }
        else
        {
            Haptics::mutex.unlock();
            xSemaphoreTake(semaphoreHandle, portMAX_DELAY);
        }
    }
    else if (characteristicUUID == METRONOME_TEMPO_UUID)
    {
        beatInterval = (60000 / *pCharacteristic->getData()) - METRONOME_BEAT_DURATION;
        preferences.putUChar("TEMPO", *pCharacteristic->getData());
    }
}

void Metronome::createService(BLEServer *pServer)
{
    preferences.begin(METRONOME_PREFS_NAMESPACE);
    pService = pServer->createService(METRONOME_SERVICE_UUID);
    semaphoreHandle = xSemaphoreCreateBinary();

    pService->addCharacteristic(&stateCharacteristic);
    stateDescriptor.setValue("Metronome State");
    stateCharacteristic.addDescriptor(&stateDescriptor);
    stateCharacteristic.setCallbacks(new Metronome::Callbacks);

    pService->addCharacteristic(&tempoCharacteristic);
    tempoDescriptor.setValue("Metronome Tempo");
    tempoCharacteristic.addDescriptor(&tempoDescriptor);
    tempoCharacteristic.setCallbacks(new Metronome::Callbacks);

    pService->start();

    // Load Previous Setting
    uint8_t tempo = preferences.getUChar("TEMPO", 1);
    tempoCharacteristic.setValue((uint8_t*)&tempo, 1);
    beatInterval = (60000 / tempo) - METRONOME_BEAT_DURATION;

    xTaskCreate(serviceTask,
                "Metronome Service",
                METRONOME_TASK_STACK_DEPTH,
                NULL,
                tskIDLE_PRIORITY,
                &taskHandle);
}

void Metronome::serviceTask(void*)
{
    while (true)
    {
        xSemaphoreTake(semaphoreHandle, portMAX_DELAY);
        Haptics::pulse(255, METRONOME_BEAT_DURATION);
        xSemaphoreGive(semaphoreHandle);
        delay(beatInterval);
    }
}