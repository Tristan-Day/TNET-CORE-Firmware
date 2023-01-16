#include <services/Metronome.hpp>

MetronomeService *MetronomeService::pInstance;

const std::string MetronomeService::SERVICE_UUID = "429b5aa8-d015-4705-99db-a51c49fc84b6";
const std::string MetronomeService::TEMPO_UUID = "726eb1ec-e713-400a-a7d3-b38a94d7095c";

void MetronomeService::onWrite(BLECharacteristic *pCharacteristic)
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
            Haptics::aquireLock();
            resume();
        }
        else
        {
            Haptics::releaseLock();
            suspend();
        }
    }
    else if (characteristicUUID == TEMPO_UUID)
    {
        setTempo(*pCharacteristic->getData());
        preferences.putUChar("TEMPO", *pCharacteristic->getData());
    }
}

void MetronomeService::task(void *)
{
    MetronomeService *self = get();

    while (true)
    {
        xSemaphoreTake(self->taskSemaphore, portMAX_DELAY);
        Haptics::vibrate(BEAT_DURATION);
        xSemaphoreGive(self->taskSemaphore);
        delay(self->beatInterval);
    }
}

MetronomeService *MetronomeService::get()
{
    if (pInstance == NULL)
    {
        pInstance = new MetronomeService();
    }
    return pInstance;
}

void MetronomeService::init(BLEServer *pServer)
{
    preferences.begin("METRONOME");
    pService = pServer->createService(SERVICE_UUID);
    taskSemaphore = xSemaphoreCreateBinary();

    pService->addCharacteristic(&stateCharacteristic);
    stateDescriptor.setValue("Metronome State");
    stateCharacteristic.addDescriptor(&stateDescriptor);
    stateCharacteristic.setCallbacks(this);

    pService->addCharacteristic(&tempoCharacteristic);
    tempoDescriptor.setValue("Metronome Tempo");
    tempoCharacteristic.addDescriptor(&tempoDescriptor);
    tempoCharacteristic.setCallbacks(this);

    pService->start();

    uint8_t tempo = preferences.getUChar("TEMPO", 1);
    setTempo(tempo);

    xTaskCreate(task, "Metronome Service", TASK_STACK_DEPTH, NULL,
        tskIDLE_PRIORITY, &taskHandle);
}

void MetronomeService::setTempo(uint8_t tempo)
{
    beatInterval = (60000 / tempo) - BEAT_DURATION;
    tempoCharacteristic.setValue((uint8_t *)&tempo, 1);
}