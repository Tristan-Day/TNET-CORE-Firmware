#include <checksum.h>

uint32_t checksum::sum(char *data, uint data_length)
{
  uint32_t sum;
  for (uint i = 0; i < data_length; i++)
  {
    sum += *data;
  }
  return sum;
}

uint16_t checksum::generate(char *data, uint data_length)
{
  uint32_t sum = checksum::sum(data, data_length);
  return 0xFFFF ^ sum;
}

bool checksum::verify(char *data, uint data_length, uint16_t checksum)
{
  checksum += checksum::sum(data, data_length);

  if (checksum == 0xFFFF)
  {
    return true;
  }
  else
  {
    return false;
  }
}