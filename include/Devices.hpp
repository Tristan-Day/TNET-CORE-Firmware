#include <Arduino.h>
#include <queue>
#include <mutex>

#include <Adafruit_BME280.h>
#include <Adafruit_MPU6050.h>
#include <QMC5883LCompass.h>
#include <Sparkfun_MAX30105.h>

#include <HeartRate.h>

#define DEFAULT_SAMPLE_INTERVAL 10000

#define BME280_ADDRESS 0x76

#define MAX30105_BUFFER_SIZE 4
#define MAX30105_TASK_STACK_DEPTH 3000
#define MAX30105_SAMPLE_SIZE 1000

#define GY8511_INPUT_PIN 26
#define GY8511_ENABLE_PIN 27
#define GY8511_REFERNCE_PIN 16
#define GY8511_BUFFER_SIZE 8
#define GY8511_TASK_STACK_DEPTH 2000
#define GY8511_SAMPLE_SIZE 5000

#define HAPTIC_OUTPUT_PIN LED_BUILTIN  // NON-FINAL - Planned Pin 25
#define HAPTIC_INPUT_PIN 15            // NON-FINAL - Planned Pin 32
#define HAPTIC_CAPACITANCE_THRESHOLD 20
#define HAPTIC_MIN_INTERVAL 150
#define HAPTIC_INPUT_MAX_INTERVAL 1000
#define HAPTIC_TIMER_STACK_DEPTH 1500

#define BATTERY_INPUT_PIN 13  // NON-FINAL - Planned Pin 35
#define BATTERY_SAMPLE_SIZE 10
#define BATTERY_FULL_VOLTAGE 4.2
#define BATTERY_EMPTY_VOLTAGE 3.0

namespace MPU6050
{
    extern Adafruit_MPU6050 device;
    extern bool connectionState;

    void initialise();
}  // namespace MPU6050

namespace BME280
{
    extern Adafruit_BME280 device;
    extern bool connectionState;

    extern long sampleInterval;

    void initialise();
}  // namespace BME280

namespace MAX30105
{
    extern Sparkfun_MAX30105 device;

    extern std::mutex mutex;
    extern TaskHandle_t taskHandle;

    extern bool connectionState;

    extern long sampleInterval;

    extern uint8_t buffer[MAX30105_BUFFER_SIZE];
    extern uint8_t bufferCounter;
    extern uint32_t lastPulse;

    extern bool contactDetected;

    void initialise();

    void serviceTask(void *);

    void pause();

    void resume();

    uint8_t getHeartRate();
}  // namespace MAX30105

namespace GY8511
{
    extern TaskHandle_t taskHandle;

    extern long sampleInterval;

    extern uint16_t buffer[GY8511_BUFFER_SIZE][2];
    extern uint8_t bufferCounter;

    void initialise();

    void serviceTask(void *);

    float getIntensity();
}  // namespace GY8511

namespace GY721
{
    extern QMC5883LCompass device;

    void initialise();
}  // namespace GY721

namespace Haptics
{
    extern std::mutex mutex;

    void initialise();

    void _touchProcessorTask(void *);

    void _touchInterrupt();

    // Lock must be aquired before use
    void pulse(uint8_t intensity, long duration);
}  // namespace Haptics

namespace Battery
{
    void initialise();

    float getVoltage();

    float getPercent();
}  // namespace Battery