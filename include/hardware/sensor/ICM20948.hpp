#pragma once

#include <Sparkfun_ICM_20948.h>

class ICM20948
{
  private:
    static ICM20948* instance;

    ICM20948();

  public:
    Sparkfun_ICM_20948_I2C* sensor;

    static ICM20948* get();

    ICM20948(ICM20948& other) = delete;

    void operator=(const ICM20948&) = delete;
};