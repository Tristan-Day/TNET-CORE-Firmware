#pragma once

#include <Adafruit_MAX1704X.h>

#include <Arduino.h>
#include <Persistence.hpp>

#include <service/Microservice.hpp>

class MAX17048
{
  private:
    static MAX17048* instance;

    Adafruit_MAX17048* sensor;

    Preferences* preferences;
    Microservice* processor;

    MAX17048();

  public:
    Persistent<long>* CHG;

    Persistent<float>* VLT;

    static MAX17048* get();

    MAX17048(MAX17048& other) = delete;

    void operator=(const MAX17048&) = delete;
};