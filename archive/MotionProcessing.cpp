#include <motion_processing.h>

void GestureRecognition::init(float motion_threshold, uint processing_timeout,
                              uint delta_minimum, uint delta_maximum)
{
    this->motion_threshold = motion_threshold;
    this->processing_timeout = processing_timeout;
    this->delta_minimum = delta_minimum;
    this->delta_maximum = delta_maximum;

    variance_cache = 0;
    delta_time = 0;
    peak_count = 1;
}

uint8_t GestureRecognition::handle_event(float current_value)
// Call this function every 10ms to handle events
{
    float variance = std::abs(current_value - variance_cache);
    variance_cache = current_value;

    if (peak_count != 1 and delta_time > processing_timeout)
    {
        uint8_t peak_count = this->peak_count;
        this->peak_count = 1;

#ifdef DEBUG_MOTION_PROCESSING
        Serial.printf("Gesture %u-tap\n", peak_count);
#endif

        return peak_count;
    }

    if (variance > motion_threshold)
    {
// #ifdef DEBUG_MOTION_PROCESSING
//         Serial.printf("Threshold Exceeded: (%u < %u < %u)\n",
//                       delta_minimum, delta_time, delta_maximum);
// #endif
        if (delta_time > delta_minimum and delta_time < delta_maximum)
        {
            this->peak_count++;
#ifdef DEBUG_MOTION_PROCESSING
            Serial.printf("Peak Count: %u Delta Time: %u\n",
                          this->peak_count, delta_time);
#endif
        }
        delta_time = 0;
    }
    else
    {
        delta_time++;
    }
    return 0;
}

void Pedometer::handle(float x, float y, float z)
{
    float vector = std::sqrt((x * x) + (y * y) + (z * z));
    float variance = vector - vector_cache;

    if (variance > 6)
    {
        step_count++;
#ifdef DEBUG_MOTION_PROCESSING
        Serial.printf("Step Count: %u\n", step_count);
#endif
    }

    vector_cache = vector;
}

void Pedometer::init(float height, bool male)
{
    if (male)
    {
        stride_length = height * 0.415;
    }
    else
    {
        stride_length = height * 0.413;
    }
}

void Pedometer::clear()
{
    step_count = 0;
}

uint Pedometer::get_distance()
{
    return step_count * stride_length;
}