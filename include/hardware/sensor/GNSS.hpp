#pragma once

#include <service/Service.hpp>

#include <SoftwareSerial.h>
#include <Sparkfun_GNSS.h>

class GNSS
{
  private:
    static constexpr uint32_t BAUDRATE = 9600;

    static GNSS* instance;

    SoftwareSerial port;

    GNSS();

  public:
    SFE_UBLOX_GNSS* sensor;

    static GNSS* get();

    GNSS(GNSS& other) = delete;

    void operator=(const GNSS&) = delete;
};