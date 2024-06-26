/**
 * @brief This is a library for the ADXL 357 Sensor.
 *
 * @author Yuzhou Shen
 *
 */

#pragma once

#include "IADXL3XX.h"


/**
 * @brief ADXL357_REGISTERS::RANGE (`0x2C`) register defination.
 * 
 * <b class="tab-title">Bit Descriptions for Range </b>
 * | Bits     | 7      | 6       | [5:2]    | [1:0] |
 * |----------|--------|---------|----------|-------|
 * | Bit Name | I2C_HS | INT_POL | Reserved | Range |
 * 
 */
enum class adxl357_range_t : uint8_t{
    I2C_HS_High_Speed_Mode = 0b10000000, /*!< I2C speed: High speed mode. */
    I2C_HS_Fast_Mode =       0b00000000, /*!< I2C speed: Fast mode. */

    INT_POL_LOW =            0b00000000, /*!< INT1 and INT2 are active **low**.  */
    INT_POL_High =           0b01000000, /*!< INT1 and INT2 are active **high**.  */

    Range_40_G =             0b00000011, /*!< +/- 40 g*/
    Range_20_G =             0b00000010, /*!< +/- 20 g*/
    Range_10_G =             0b00000001, /*!< +/- 10 g*/
};



/**
 * @brief ADXL357 class to initialize, sample data, and etc. of the ADXL357 Sensor.
 *
 */
class ADXL357: public IADXL3XX {
public:
    //! An scale scture.
    /*! To store scale factors in different ranges.
        \sa Scale
    */
    struct scale_t{
        double _10G;
        double _20G;
        double _40G;
    };

private:
    /**
     * @brief An scale_t static member.
     * Stored scale factors in different ranges.
    */
    static scale_t Scale;

public:
    /**
     * @brief Construct a new ADXL357 object
     * 
     * @param isASELHigh 
     */
    ADXL357(bool isASELHigh = false);

    virtual bool setRange(adxl357_range_t range);

    virtual double getScale();

};
