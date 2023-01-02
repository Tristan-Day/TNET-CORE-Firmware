#include <Arduino.h>

namespace checksum
{

    uint32_t sum(char *data, uint data_length);

    uint16_t generate(char *data, uint data_length);

    bool verify(char *data, uint data_length, uint16_t checksum);

}