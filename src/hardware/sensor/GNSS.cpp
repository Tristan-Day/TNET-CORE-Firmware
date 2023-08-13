#include <hardware/sensor/GNSS.hpp>

GNSS* GNSS::instance = nullptr;

GNSS::GNSS()
{
    port.begin(BAUDRATE, SWSERIAL_8N1, RX, TX);

    sensor = new SFE_UBLOX_GNSS();
    sensor->begin(port);

#ifdef DEBUG
    sensor->enableDebugging();
#endif
};

GNSS* GNSS::get()
{
    if (instance == nullptr)
    {
        instance = new GNSS();
    }
    return instance;
}
