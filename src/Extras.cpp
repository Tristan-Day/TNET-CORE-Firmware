#include <Extras.hpp>

const char* getResetReason(uint8_t reason)
{
    switch (reason)
    {
    case 1:
        return "VBAT power on reset";

    case 3:
        return "Software reset digital core";

    case 4:
        return "Legacy watch dog reset digital core";

    case 5:
        return "Deep Sleep reset digital core";

    case 6:
        return "Reset by SLC module, reset digital core";

    case 7:
        return "Timer Group0 Watch dog reset digital core";

    case 8:
        return "Timer Group1 Watch dog reset digital core";

    case 9:
        return "RTC Watch dog Reset digital core";

    case 10:
        return "Instrusion tested to reset CPU";

    case 11:
        return "Time Group reset CPU";

    case 12:
        return "Software reset CPU";

    case 13:
        return "RTC Watch dog Reset CPU";

    case 14:
        return "For APP CPU, reseted by PRO CPU";

    case 15:
        return "Reset when the vdd voltage is not stable";

    case 16:
        return "RTC Watch dog reset digital core and RTC module";

    default:
        return "No Meaning";
    }
}