#pragma once
#include "Arduino.h"


/**
 * @brief This is a enum type to store the address of register in ADXL3XX
 *
 */
enum class ADXL3XX_REGISTERS : uint8_t{
    DEVID_AD  = 0x00,
    DEVID_MST = 0x01,
    PARTID    = 0x02, /*!< Device ID (0xED)     */ 
    REVID     = 0x03, /*!< Mask revision (0x01) */ 

    STATUS    = 0x04, /*!< This register includes bits that 
                           describe the various conditions 
                           of the ADXL3XX.                   */

    FIFO_ENTRIES = 0x05, /*!< Number of data samples stored in the FIFO  */
    TEMP2        = 0x06, /*!< Uncalibrated temperature data              */
    TEMP1        = 0x07,

    XDATA3       = 0x08, /*!< X-axis data */
    XDATA2       = 0x09,
    XDATA1       = 0x0A,

    YDATA3       = 0x0B, /*!< Y-axis data */
    YDATA2       = 0x0C,
    YDATA1       = 0x0D,

    ZDATA3       = 0x0E, /*!< Z-axis data */
    ZDATA2       = 0x0F,
    ZDATA1       = 0x10,

    FIFO_DATA    = 0x11,

    OFFSET_X_H   = 0x1E, /*!< Offset added to x-axis data after all
                              other signal processing.              */
    OFFSET_X_L   = 0x1F,

    OFFSET_Y_H   = 0x20, /*!< Offset added to y-axis data after all 
                              other signal processing.              */
    OFFSET_Y_L   = 0x21,

    OFFSET_Z_H   = 0x22, /*!< Offset added to z-axis data after all 
                              other signal processing.              */
    OFFSET_Z_L   = 0x23,

    ACT_EN        = 0x24,
    ACT_THRESH_H  = 0x25,
    ACT_THRESH_L  = 0x26,
    ACT_COUNT     = 0x27,

    FILTER        = 0x28, /*!< Use this register to specify parameters for the internal high-pass and low-pass filters.                         */
    FIFO_SAMPLES  = 0x29, /*!< Watermark number of samples stored in the FIFO that triggers a FIFO_FULL condition. Values range from 1 to 96.   */
    INT_MAP       = 0x2A,
    SYNC          = 0x2B, /*!< Use this register to control the external timing triggers                                                        */
    RANGE         = 0x2C,
    POWER_CTL     = 0x2D,
    SELF_TEST     = 0x2E,
    RESET         = 0x2F,
};

/**
 * @brief ADXL3XX_REGISTERS::RANGE (`0x2C`) register defination.
 * 
 * <b class="tab-title">Bit Descriptions for Range </b>
 * | Bits     | 7      | 6       | [5:2]    | [1:0] |
 * |----------|--------|---------|----------|-------|
 * | Bit Name | I2C_HS | INT_POL | Reserved | Range |
 * 
 */
enum class iadxl3xx_range_t : uint8_t{
    I2C_HS_High_Speed_Mode = 0b10000000, /*!< I2C speed: High speed mode. */
    I2C_HS_Fast_Mode =       0b00000000, /*!< I2C speed: Fast mode. */

    INT_POL_LOW =            0b00000000, /*!< INT1 and INT2 are active **low**.  */
    INT_POL_High =           0b01000000, /*!< INT1 and INT2 are active **high**.  */
};

/**
 * @brief ADXL357_REGISTERS::POWER_CTL (`0x2D`) register defination.
 * 
 * <b class="tab-title">Bit Descriptions for power control </b>
 * | Bits     | [7:3]      | 2        | 1         | 0        |
 * |----------|------------|----------|-----------|----------|
 * | Bit Name |  Reserved  | DRDY_OFF |  TEMP_OFF |  Standby |
 * 
 */
enum class iadxl3xx_power_ctl_t : uint8_t{
    DRDY_OFF_DEFAULT =       0b00000000,
    DRDY_OFF_0       =       0b00000100,

    TEMP_OFF_Enable  =       0b00000000, /*!< Enable temperature processing. */
    TEMP_OFF_Disable =       0b00000010, /*!< Disable temperature processing. Temperature processing is also disabled when standby = 1.*/

    Standby_Measurement =    0b00000000,
    Standby_Standby =        0b00000001,
};


/**
 * @brief ADXL357_REGISTERS::FILTER (`0x28`) register defination.
 * 
 * <b class="tab-title">Bit Descriptions for power control </b>
 * | Bits        | 7        | [6:4]                                                                         | [3:0]                          |
 * |-------------|----------|-------------------------------------------------------------------------------|--------------------------------|
 * | Bit Name    | Reserved | HPF_CORNER                                                                    | ODR_LPF                        |
 * | Description |          | −3 dB filter corner for the first-order, high-pass filter relative to the ODR | ODR and low-pass filter corner |
 * 
 */
enum class iadxl3xx_filter_t : uint8_t{
    HPF_CORNER_NO_HPF    = 0b00000000,
    HPF_CORNER_24_7EN4   = 0b00010000, /*!< 24.7 × 10−4 × ODR */
    HPF_CORNER_6_2084EN4 = 0b00100000, /*!< 6.2084  × 10−4 × ODR */
    HPF_CORNER_1_5545EN4 = 0b00110000, /*!< 1.5545  × 10−4 × ODR */
    HPF_CORNER_0_3862EN4 = 0b01000000, /*!< 0.3862  × 10−4 × ODR */
    HPF_CORNER_0_0964EN4 = 0b01010000, /*!< 0.0954  × 10−4 × ODR */
    HPF_CORNER_0_0238EN4 = 0b01100000, /*!< 0.0238  × 10−4 × ODR */

    ODR_LPF_4000         = 0b00000000, /*!< ODR: 4000 Hz and LPF: 1000 Hz */
    ODR_LPF_2000         = 0b00000001, /*!< ODR: 1000 Hz and LPF: 500 Hz */
    ODR_LPF_1000         = 0b00000010, /*!< ODR: 1000 Hz and LPF: 250 Hz */
    ODR_LPF_500          = 0b00000011, /*!< ODR: 500 Hz and LPF: 125 Hz */
    ODR_LPF_250          = 0b00000100, /*!< ODR: 250 Hz and LPF: 62.5 Hz */
    ODR_LPF_125          = 0b00000101, /*!< ODR: 125 Hz and LPF: 31.25 Hz */
    ODR_LPF_62_5         = 0b00000110, /*!< ODR: 62.5 Hz and LPF: 15.625 Hz */
    ODR_LPF_31_25        = 0b00000111, /*!< ODR: 31.25 Hz and LPF: 7.813 Hz */
    ODR_LPF_15_625       = 0b00001000, /*!< ODR: 15.625 Hz and LPF: 3.906 Hz */
    ODR_LPF_7_813        = 0b00001001, /*!< ODR: 7.813 Hz and LPF: 1.953 Hz */
    ODR_LPF_3_906        = 0b00001010, /*!< ODR: 3.906 Hz and LPF: 0.977 Hz */
};

class IADXL3XX {
public:
    /**
     * @brief Construct a new ADXL357 object
     * 
     * @param isASELHigh 
     */
    IADXL3XX(bool isASELHigh = false);

    virtual bool begin();
    virtual bool setRange(iadxl3xx_range_t range);
    virtual bool setRange(uint8_t range);
    virtual uint8_t getRange();

    virtual bool setFilter(iadxl3xx_filter_t filter);
    virtual bool setFilter(uint8_t filter);
    virtual uint8_t getFilter();

    virtual double getHPF();
    virtual double getODR();
    virtual double getScale()=0;

    virtual void setMode(bool isMeasurmentMode);

    virtual uint8_t getAnalogDeviceID();
    virtual uint8_t getDeviceMEMSID();
    virtual uint8_t getDeviceID();
    virtual uint8_t getMaskRevision();

    virtual void writeRegister(const ADXL3XX_REGISTERS& reg, uint8_t value);
    virtual uint8_t readRegister(const ADXL3XX_REGISTERS& reg);
    virtual int32_t read20(const ADXL3XX_REGISTERS& reg);

    virtual int32_t getX();
    virtual int32_t getY();
    virtual int32_t getZ();
   /**
    * @brief Get Acceleration on X,Y,Z axis.
    * 
    * @param x the output of x.
    * @param y the output of y.
    * @param z the output of z.
    * @return true: Successful.
    * @return false: failed.
    */
    virtual bool    getXYZ(int32_t &x, int32_t &y, int32_t &z);

    virtual uint8_t getFIFOEntries();

    virtual bool    getFIFOData(int32_t &x, int32_t &y, int32_t &z);

    virtual int8_t getAllFIFOData(int32_t data[]);

    virtual int8_t getAllFIFOData(int32_t x[], int32_t y[], int32_t z[]);

    virtual int8_t getNFIFOData(int8_t n, int32_t x[], int32_t y[], int32_t z[]);

protected:
    uint8_t _i2caddr;

    uint8_t _i2c_speed;
    uint8_t _int_pol;
    uint8_t _range;

    uint8_t _drdy_off;
    uint8_t _temp_off;
    uint8_t _standby;
};
