#include <Arduino.h>

#define DEBUG_MOTION_PROCESSING

class GestureRecognition
{
private:
    uint processing_timeout;
    float motion_threshold;
    uint delta_minimum;
    uint delta_maximum;

    float variance_cache;
    uint delta_time;
    uint8_t peak_count;

public:
    void init(float motion_threshold, uint processing_timeout, uint delta_minimum, uint delta_maximum);

    uint8_t handle_event(float current_value);
};

class Pedometer
{
private:
    float stride_length;
    float vector_cache;

public:
    uint step_count;

    void init(float height, bool male);

    void handle(float x, float y, float z);

    void clear();

    uint get_distance();
};