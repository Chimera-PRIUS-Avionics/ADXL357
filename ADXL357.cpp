#include "Arduino.h"
#include <Wire.h>

#include "ADXL357.h"



ADXL357::scale_t ADXL357::Scale = {
    ._10G = 19.5e-6,
    ._20G = 39e-6,
    ._40G = 78e-6,
};

ADXL357::ADXL357(bool isASELHigh) : IADXL3XX(isASELHigh)
{
    if (isASELHigh == false)
    {
        _i2caddr = 0x1D;
    }
    else
    {
        _i2caddr = 0x53;
    }

    this->_range = static_cast<uint8_t>(adxl357_range_t::Range_40_G);
}


bool ADXL357::setRange(adxl357_range_t range)
{
    return IADXL3XX::setRange(static_cast<uint8_t>(range));
}


double ADXL357::getScale()
{
    switch (static_cast<adxl357_range_t>(this->getRange()))
    {
    case adxl357_range_t::Range_40_G:
        return ADXL357::Scale._40G;
    case adxl357_range_t::Range_20_G:
        return ADXL357::Scale._20G;
    case adxl357_range_t::Range_10_G:
        return ADXL357::Scale._10G;
    }
}
