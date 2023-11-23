// This is a library for the ADXL 357 Sensor.
//
//    Author: Yuzhou Shen
//    File: ADXL357.h
//
#ifndef ADXL357_H
#define ADXL357_H

#include "Arduino.h"

// Registers
#define ADXL357_REG_DEVID_AD (0x00)         // Analog Devices ID (0xAD) 
#define ADXL357_REG_DEVID_MST (0x01)        // Analog Devices MEMS ID (0x1D)

#define ADXL357_REG_PARTID (0x02)           // Device ID (0xED) 
#define ADXL357_REG_REVID (0x03)            // Mask revision (0x01)

#define ADXL357_REG_STATUS (0x04)           // This register includes bits that 
                                            // describe the various conditions 
                                            // of the ADXL357. 
#define ADXL357_REG_FIFO_ENTRIES (0x05)     // Number of data samples stored in the FIFO 
#define ADXL357_REG_TEMP2 (0x06)            // Uncalibrated temperature data 
#define ADXL357_REG_TEMP1 (0x07)

#define ADXL357_REG_XDATA3 (0x08)           // X-axis data
#define ADXL357_REG_XDATA2 (0x09)
#define ADXL357_REG_XDATA1 (0x0A)

#define ADXL357_REG_YDATA3 (0x0B)           // Y-axis data
#define ADXL357_REG_YDATA2 (0x0C)
#define ADXL357_REG_YDATA1 (0x0D)

#define ADXL357_REG_ZDATA3 (0x0E)           // Z-axis data
#define ADXL357_REG_ZDATA2 (0x0F)
#define ADXL357_REG_ZDATA1 (0x10)

#define ADXL357_REG_FIFO_DATA (0x11)

#define ADXL357_REG_OFFSET_X_H (0x1E)       // Offset added to x-axis data after all 
                                            // other signal processing.
#define ADXL357_REG_OFFSET_X_L (0x1F)

#define ADXL357_REG_OFFSET_Y_H (0x20)       // Offset added to y-axis data after all 
                                            // other signal processing.
#define ADXL357_REG_OFFSET_Y_L (0x21)

#define ADXL357_REG_OFFSET_Z_H (0x22)       // Offset added to z-axis data after all 
                                            // other signal processing.
#define ADXL357_REG_OFFSET_Z_L (0x23)

#define ADXL357_REG_ACT_EN (0x24)
#define ADXL357_REG_ACT_THRESH_H (0x25)
#define ADXL357_REG_ACT_THRESH_L (0x26)
#define ADXL357_REG_ACT_COUNT (0x27)

#define ADXL357_REG_FILTER (0x28)           // Use this register to specify parameters for the internal high-pass and low-pass filters. 
#define ADXL357_REG_FIFO_SAMPLES (0x29)     // Watermark number of samples stored in the FIFO that triggers a FIFO_FULL condition. Values range from 1 to 96. 
#define ADXL357_REG_INT_MAP (0x2A)
#define ADXL357_REG_SYNC (0x2B)             // Use this register to control the external timing triggers
#define ADXL357_REG_RANGE (0x2C)
#define ADXL357_REG_POWER_CTL (0x2D)
#define ADXL357_REG_SELF_TEST (0x2E)
#define ADXL357_REG_RESET (0x2F)


// Scale Fractor
#define ADXL357_SCL_10G (19.5e-6)
#define ADXL357_SCL_20G (39e-6)
#define ADXL357_SCL_40G (78e-6)

// Ranges
enum class adxl357_range_t : uint8_t{
    I2C_HS_High_Speed_Mode = 0b10000000,
    I2C_HS_Fast_Mode =       0b00000000,

    INT_POL_LOW =            0b00000000,
    INT_POL_High =           0b01000000,

    Range_40_G =             0b00000011,
    Range_20_G =             0b00000010,
    Range_10_G =             0b00000001,
};

// Power Contrl
enum class adxl357_power_ctl_t : uint8_t{
    DRDY_OFF_DEFAULT =       0b00000000,
    DRDY_OFF_0       =       0b00000100,

    TEMP_OFF_Enable  =       0b00000000,
    TEMP_OFF_Disable =       0b00000010,

    Standby_Measurement =    0b00000000,
    Standby_Standby =        0b00000001,
};

class ADXL357 {
public:
    ADXL357(bool isASELHigh = false);

    bool begin();
    bool setRange(adxl357_range_t range);
    adxl357_range_t getRange();

    void setMode(bool isMeasurmentMode);

    uint8_t getAnalogDeviceID();
    uint8_t getDeviceMEMSID();
    uint8_t getDeviceID();
    uint8_t getMaskRevision();

    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
    int32_t read20(uint8_t reg);

    int32_t getX();
    int32_t getY();
    int32_t getZ();
    bool getXYZ(int32_t &x, int32_t &y, int32_t &z);

protected:
    uint8_t _i2caddr;

    adxl357_range_t _i2c_speed;
    adxl357_range_t _int_pol;
    adxl357_range_t _range;

    adxl357_power_ctl_t _drdy_off;
    adxl357_power_ctl_t _temp_off;
    adxl357_power_ctl_t _standby;
};

#endif
