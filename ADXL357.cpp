#include "Arduino.h"
#include <Wire.h>

#include "ADXL357.h"

ADXL357::ADXL357(bool isASELHigh):
    _i2c_speed(adxl357_range_t::I2C_HS_High_Speed_Mode),
    _int_pol(adxl357_range_t::INT_POL_LOW),
    _range(adxl357_range_t::Range_10_G),

    _drdy_off(adxl357_power_ctl_t::DRDY_OFF_DEFAULT),
    _temp_off(adxl357_power_ctl_t::TEMP_OFF_Enable),
    _standby(adxl357_power_ctl_t::Standby_Standby)
    {
    if(isASELHigh == false){
        _i2caddr = 0x1D;
    }else{
        _i2caddr = 0x53;
    }
}


bool ADXL357::begin() {
    Wire.begin();

    uint8_t analogDID = getAnalogDeviceID();
    uint8_t memsDID = getDeviceMEMSID();
    uint8_t DID = getDeviceID();
    uint8_t RevID = getMaskRevision();

    Serial.print("analogDID: 0x");
    Serial.println(analogDID, HEX);
    
    Serial.print("memsDID: 0x");
    Serial.println(memsDID, HEX);

    Serial.print("DID: 0x");
    Serial.println(DID, HEX);

    Serial.print("RevID: 0x");
    Serial.println(RevID, HEX);

    if (analogDID != 0xAD) {
        return false;
    }
    if (memsDID != 0x1D) {
        return false;
    }
    if (DID != 0xED) {
        return false;
    }
    if (RevID != 0x01) {
        return false;
    }

    setRange(_range);

    setMode(true);

    return true;
}

bool ADXL357::setRange(adxl357_range_t range){
    if(_standby == adxl357_power_ctl_t::Standby_Measurement){
        return false;
    }

    _range = range;

    writeRegister(ADXL357_REG_RANGE, 
    static_cast<uint8_t>(_i2c_speed) |
    static_cast<uint8_t>(_int_pol) |
    static_cast<uint8_t>(_range));

    return true;
}

adxl357_range_t ADXL357::getRange(){
    return _range;
}

void ADXL357::setMode(bool isMeasurmentMode){
    if(isMeasurmentMode == true){
        _standby = adxl357_power_ctl_t::Standby_Measurement;
    }else{
        _standby = adxl357_power_ctl_t::Standby_Standby;
    }

    writeRegister(ADXL357_REG_POWER_CTL, 
    static_cast<uint8_t>(_drdy_off) |
    static_cast<uint8_t>(_temp_off) | 
    static_cast<uint8_t>(_standby));
}


uint8_t ADXL357::getAnalogDeviceID(){
    return readRegister(ADXL357_REG_DEVID_AD);
}


uint8_t ADXL357::getDeviceMEMSID(){
    return readRegister(ADXL357_REG_DEVID_MST);
}

uint8_t ADXL357::getDeviceID(){
    return readRegister(ADXL357_REG_PARTID);
}

uint8_t ADXL357::getMaskRevision(){
    return readRegister(ADXL357_REG_REVID);
}

void ADXL357::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(this->_i2caddr);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t ADXL357::readRegister(uint8_t reg) {
    Wire.beginTransmission(this->_i2caddr);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(this->_i2caddr, uint8_t(1));
    uint8_t val = Wire.read();
  return val;
}

int32_t ADXL357::read20(uint8_t reg) {
    Wire.beginTransmission(this->_i2caddr);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(this->_i2caddr, uint8_t(3));
    uint8_t data3 = Wire.read();
    uint8_t data2 = Wire.read();
    uint8_t data1 = Wire.read();

    int32_t val = static_cast<int32_t>(data3) << 16;
    val |= (static_cast<int32_t>(data2) << 8);
    val |= (static_cast<int32_t>(data1) & 0xF0);

    val = (val<<8)>>12;
    return static_cast<int32_t>(val);
}



int32_t ADXL357::getX() { 
    return read20(ADXL357_REG_XDATA3); 
}

int32_t ADXL357::getY() {
    return read20(ADXL357_REG_YDATA3);
}

int32_t ADXL357::getZ() {
    return read20(ADXL357_REG_ZDATA3);
}

bool ADXL357::getXYZ(int32_t &x, int32_t &y, int32_t &z) {
    x = getX();
    y = getY();
    z = getZ();
    return true;
}
