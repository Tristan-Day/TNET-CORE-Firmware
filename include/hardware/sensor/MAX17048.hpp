#pragma once

#include <Adafruit_MAX1704X.h>

class MAX17048
{
  private:
    static MAX17048* instance;

    MAX17048();

  public:
    Adafruit_MAX17048* sensor;

    static MAX17048* get();

    MAX17048(MAX17048& other) = delete;

    void operator=(const MAX17048&) = delete;
};