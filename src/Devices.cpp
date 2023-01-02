#include <Devices.hpp>

#define DEBUG

namespace MPU6050
{
    Adafruit_MPU6050 device;
    bool connectionState;

    std::mutex mutex;
}  // namespace MPU6050

void MPU6050::initialise()
{
    connectionState = MPU6050::device.begin();
}

namespace BME280
{
    Adafruit_BME280 device;
    bool connectionState;

    long sampleInterval = DEFAULT_SAMPLE_INTERVAL;
}  // namespace BME280

void BME280::initialise()
{
    connectionState = BME280::device.begin(BME280_ADDRESS);
    BME280::device.setSampling(Adafruit_BME280::MODE_FORCED);

#ifdef DEBUG
    if (not connectionState)
    {
        Serial.println("[ERROR] BME280 not found");
    }
#endif
}

namespace MAX30105
{
    Sparkfun_MAX30105 device;

    std::mutex mutex;
    TaskHandle_t taskHandle;

    bool connectionState;

    long sampleInterval = DEFAULT_SAMPLE_INTERVAL;

    uint8_t buffer[MAX30105_BUFFER_SIZE];
    uint8_t bufferCounter = 0;
    uint32_t lastPulse = 0;

    bool contactDetected = false;
}  // namespace MAX30105

void MAX30105::initialise()
{
    connectionState = MAX30105::device.begin(Wire, I2C_SPEED_STANDARD);

#ifdef DEBUG
    if (not connectionState)
    {
        Serial.println("[ERROR] MAX30105 not found");
    }
#endif

    MAX30105::device.setup();

    xTaskCreate(serviceTask, "MAX30105 Service", MAX30105_TASK_STACK_DEPTH,
                NULL, tskIDLE_PRIORITY, &taskHandle);
}

void MAX30105::serviceTask(void *)
{
    while (true)
    {
        mutex.lock();
        MAX30105::device.wakeUp();
        for (long i = 0; i < MAX30105_SAMPLE_SIZE; i++)
        {
            uint32_t irSample = MAX30105::device.getIR();
            if (irSample < 2000)
            {
                contactDetected = false;
            }
            else if (checkForBeat(irSample))
            {
                contactDetected = true;

                long delta = millis() - lastPulse;
                lastPulse = millis();

                float bpm = 60 / (delta / 1000.0);
                if (bpm < 255 and bpm > 20)
                {
                    buffer[bufferCounter++] = (uint8_t)bpm;
                    bufferCounter %= MAX30105_BUFFER_SIZE;
                }
            }
        }
        MAX30105::device.shutDown();
        mutex.unlock();
        delay(sampleInterval);
    }
}

void MAX30105::pause()
{
    mutex.lock();
    vTaskSuspend(taskHandle);
    mutex.unlock();
}

void MAX30105::resume()
{
    vTaskResume(taskHandle);
}

uint8_t MAX30105::getHeartRate()
{
    long sum = 0;
    for (uint8_t i = 0; i < MAX30105_BUFFER_SIZE; i++)
    {
        sum += buffer[i];
    }
    return sum / MAX30105_BUFFER_SIZE;
}

namespace GY8511
{
    TaskHandle_t taskHandle;

    long sampleInterval = DEFAULT_SAMPLE_INTERVAL;

    uint16_t buffer[GY8511_BUFFER_SIZE][2];
    uint8_t bufferCounter = 0;
}  // namespace GY8511

void GY8511::initialise()
{
    pinMode(GY8511_INPUT_PIN, INPUT);
    pinMode(GY8511_ENABLE_PIN, OUTPUT);
    pinMode(GY8511_REFERNCE_PIN, INPUT);

    xTaskCreate(serviceTask, "GY811 Service", GY8511_TASK_STACK_DEPTH, NULL,
                tskIDLE_PRIORITY, &taskHandle);
}

void GY8511::serviceTask(void *)
{
    while (true)
    {
        for (long i = 0; i < GY8511_SAMPLE_SIZE; i++)
        {
            bufferCounter++;

            digitalWrite(GY8511_ENABLE_PIN, HIGH);

            buffer[bufferCounter][0] = analogRead(GY8511_REFERNCE_PIN);
            buffer[bufferCounter][1] = analogRead(GY8511_INPUT_PIN);

            digitalWrite(GY8511_ENABLE_PIN, LOW);

            bufferCounter %= GY8511_BUFFER_SIZE;

            delay(1);
        }
        delay(sampleInterval);
    }
}

float GY8511::getIntensity()
{
    long referenceVoltage = 0;
    float inputVoltage = 0;

    for (uint8_t i = 0; i < GY8511_BUFFER_SIZE; i++)
    {
        referenceVoltage += buffer[i][0];
        inputVoltage += buffer[i][1];
    }

    // Derive accurate value from reference 3v3 pin
    inputVoltage = 3.3 / referenceVoltage * inputVoltage;

    // Derrive UV intensity though voltage mapping
    return (inputVoltage - 0.99) * 15.0 / 14.01;
}

void GY721::initialise()
{
    GY721::device.init();

    // GY721::device.setMode()
}

namespace Haptics
{
    std::mutex mutex;

    TaskHandle_t timerHandle;
    std::queue<uint8_t> eventQueue;
    uint8_t interruptCounter = 0;
    bool timerActive = false;
    long deltaTime = 0;
}  // namespace Haptics

void Haptics::initialise()
{
    // Output
    pinMode(HAPTIC_OUTPUT_PIN, OUTPUT);
    digitalWrite(HAPTIC_OUTPUT_PIN, HIGH);

    // Input
    touchAttachInterrupt(HAPTIC_INPUT_PIN, _touchInterrupt,
                         HAPTIC_CAPACITANCE_THRESHOLD);
    touchSetCycles(0x2000, 0x1000);

}

void Haptics::_touchProcessorTask(void *)
{
    // Aquire the semmaphore
    // 
}

void Haptics::_touchInterrupt()
{
#ifdef DEBUG
    Serial.println("[INFO] Haptics - Capacitance Sense");
#endif

    // Release the semmaphore

}

// NON-FINAL
void Haptics::pulse(uint8_t intensity, long duration)
{
    // analogWrite(HAPTIC_OUTPUT_PIN, intesnity);
    digitalWrite(HAPTIC_OUTPUT_PIN, LOW);

    delay(duration);

    digitalWrite(HAPTIC_OUTPUT_PIN, HIGH);
    // analogWrite(HAPTIC_OUTPUT_PIN, 0);
}

void Battery::initialise()
{
    pinMode(BATTERY_INPUT_PIN, INPUT);
}

float Battery::getVoltage()
{
    float voltage = 0;
    for (uint8_t i = 0; i < BATTERY_SAMPLE_SIZE; i++)
    {
        voltage += analogReadMilliVolts(BATTERY_INPUT_PIN);
        delay(1);
    }
    voltage /= BATTERY_SAMPLE_SIZE;

    return (voltage * 2) / 1000;  // Multiply by 2 and Convert from Mv to V
}

float Battery::getPercent()
{
    float voltage = Battery::getVoltage();

#ifdef DEBUG
    Serial.print("[INFO] Battery Voltage: ");
    Serial.println(voltage);
#endif

    return (voltage - BATTERY_EMPTY_VOLTAGE) * 100 /
           (BATTERY_FULL_VOLTAGE - BATTERY_EMPTY_VOLTAGE);
}