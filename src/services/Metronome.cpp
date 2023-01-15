#include <services/Metronome.hpp>

Metronome *Metronome::pInstance;

void Metronome::onWrite(BLECharacteristic *pCharacteristic)
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
    else if (characteristicUUID == METRONOME_TEMPO_UUID)
    {
        setTempo(*pCharacteristic->getData());
        preferences.putUChar("TEMPO", *pCharacteristic->getData());
    }
}

void Metronome::task(void *)
{
    Metronome *self = get();

    while (true)
    {
        xSemaphoreTake(self->taskSemaphore, portMAX_DELAY);
        Haptics::vibrate(BEAT_DURATION);
        xSemaphoreGive(self->taskSemaphore);
        delay(self->beatInterval);
    }
}

Metronome *Metronome::get()
{
    if (pInstance == NULL)
    {
        pInstance = new Metronome();
    }
    return pInstance;
}

void Metronome::init(BLEServer *pServer)
{
    preferences.begin("METRONOME");
    pService = pServer->createService(METRONOME_SERVICE_UUID);
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

void Metronome::setTempo(uint8_t tempo)
{
    beatInterval = (60000 / tempo) - BEAT_DURATION;
    tempoCharacteristic.setValue((uint8_t *)&tempo, 1);
}