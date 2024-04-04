#include "Arduino.h"
#include <Wire.h>

#include "ADXL355.h"



ADXL355::scale_t ADXL355::Scale = {
    ._2G = 3.9e-6,
    ._4G = 7.8e-6,
    ._8G = 15.6e-6,
};

ADXL355::ADXL355(bool isASELHigh) : IADXL3XX(isASELHigh)
{
    if (isASELHigh == false)
    {
        _i2caddr = 0x1D;
    }
    else
    {
        _i2caddr = 0x53;
    }

    this->_range = static_cast<uint8_t>(adxl355_range_t::Range_8_G);
}


bool ADXL355::setRange(adxl355_range_t range)
{
    return IADXL3XX::setRange(static_cast<uint8_t>(range));
}


double ADXL355::getScale()
{
    switch (static_cast<adxl355_range_t>(this->getRange()))
    {
    case adxl355_range_t::Range_8_G:
        return ADXL355::Scale._8G;
    case adxl355_range_t::Range_4_G:
        return ADXL355::Scale._4G;
    case adxl355_range_t::Range_2_G:
        return ADXL355::Scale._2G;
    }
}
