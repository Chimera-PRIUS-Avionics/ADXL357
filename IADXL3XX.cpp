#include "IADXL3XX.h"

#include <Wire.h>

inline void Read20BitInt(int32_t *x)
{
    *x = static_cast<int32_t>(Wire.read()) << 16;
    *x |= (static_cast<int32_t>(Wire.read()) << 8);
    *x |= (static_cast<int32_t>(Wire.read()) & 0xF0);
    *x = (*x << 8) >> 12;
}

IADXL3XX::IADXL3XX(bool isASELHigh):
    _i2c_speed(static_cast<uint8_t>(iadxl3xx_range_t::I2C_HS_Fast_Mode)),
    _int_pol(static_cast<uint8_t>(iadxl3xx_range_t::INT_POL_LOW)),
    _range(0),

    _drdy_off(static_cast<uint8_t>(iadxl3xx_power_ctl_t::DRDY_OFF_DEFAULT)),
    _temp_off(static_cast<uint8_t>(iadxl3xx_power_ctl_t::TEMP_OFF_Enable)),
    _standby(static_cast<uint8_t>(iadxl3xx_power_ctl_t::Standby_Standby))
    {
    if(isASELHigh == false){
        _i2caddr = 0x1D;
    }else{
        _i2caddr = 0x53;
    }
}


bool IADXL3XX::begin(){
    Wire.begin();

    writeRegister(ADXL3XX_REGISTERS::RESET, 1u);

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

bool IADXL3XX::setRange(iadxl3xx_range_t range){
    return setRange(static_cast<uint8_t>(range));
}

bool IADXL3XX::setRange(uint8_t range){
    if(_standby == static_cast<uint8_t>(iadxl3xx_power_ctl_t::Standby_Measurement)){
        return false;
    }

    _range = range;

    writeRegister(ADXL3XX_REGISTERS::RANGE, 
    _i2c_speed |
    _int_pol |
    _range);

    return true;
}

uint8_t IADXL3XX::getRange(){
    return _range;
}

bool IADXL3XX::setFilter(iadxl3xx_filter_t filter){
    return setFilter(static_cast<uint8_t>(filter));
}

bool IADXL3XX::setFilter(uint8_t filter){
   if(_standby == static_cast<uint8_t>(iadxl3xx_power_ctl_t::Standby_Measurement)){
        return false;
    }

    writeRegister(ADXL3XX_REGISTERS::FILTER, filter);

    return true;
}

uint8_t IADXL3XX::getFilter(){
    return readRegister(ADXL3XX_REGISTERS::FILTER);
}

double IADXL3XX::getHPF(){
    iadxl3xx_filter_t filter = static_cast<iadxl3xx_filter_t>(
        this->getFilter() & 0b01110000);

    switch (filter)
    {
    case iadxl3xx_filter_t::HPF_CORNER_NO_HPF:
        return 0;
    case iadxl3xx_filter_t::HPF_CORNER_24_7EN4:
        return 24.7e-4 * this->getODR();
    case iadxl3xx_filter_t::HPF_CORNER_6_2084EN4:
        return 6.2084e-4 * this->getODR();
    case iadxl3xx_filter_t::HPF_CORNER_1_5545EN4:
        return 1.5545e-4 * this->getODR();
    case iadxl3xx_filter_t::HPF_CORNER_0_3862EN4:
        return 0.3862e-4 * this->getODR();
    case iadxl3xx_filter_t::HPF_CORNER_0_0964EN4:
        return 0.0954e-4 * this->getODR();
    case iadxl3xx_filter_t::HPF_CORNER_0_0238EN4:
        return 0.0238e-4 * this->getODR();
    default:
        return -1;
    }
}

double IADXL3XX::getODR(){
    iadxl3xx_filter_t filter = static_cast<iadxl3xx_filter_t>(
        this->getFilter() & 0b00001111);

    switch (filter)
    {
        case iadxl3xx_filter_t::ODR_LPF_4000         :
            return 4000;
        case iadxl3xx_filter_t::ODR_LPF_2000         :
            return 2000;
        case iadxl3xx_filter_t::ODR_LPF_1000         :
            return 1000;
        case iadxl3xx_filter_t::ODR_LPF_500          :
            return 500;
        case iadxl3xx_filter_t::ODR_LPF_250          :
            return 250;
        case iadxl3xx_filter_t::ODR_LPF_125          :
            return 125;
        case iadxl3xx_filter_t::ODR_LPF_62_5         :
            return 62.5;
        case iadxl3xx_filter_t::ODR_LPF_31_25        :
            return 31.25;
        case iadxl3xx_filter_t::ODR_LPF_15_625       :
            return 15.625;
        case iadxl3xx_filter_t::ODR_LPF_7_813        :
            return 7.813;
        case iadxl3xx_filter_t::ODR_LPF_3_906        :
            return 3.906;
        default:
            return -1;
    }
}


void IADXL3XX::setMode(bool isMeasurmentMode){
    if(isMeasurmentMode == true){
        _standby = static_cast<uint8_t>(iadxl3xx_power_ctl_t::Standby_Measurement);
    }else{
        _standby = static_cast<uint8_t>(iadxl3xx_power_ctl_t::Standby_Standby);
    }

    writeRegister(ADXL3XX_REGISTERS::POWER_CTL, 
    _drdy_off |
    _temp_off | 
    _standby);
}

uint8_t IADXL3XX::getAnalogDeviceID(){
    return readRegister(ADXL3XX_REGISTERS::DEVID_AD);
}


uint8_t IADXL3XX::getDeviceMEMSID(){
    return readRegister(ADXL3XX_REGISTERS::DEVID_MST);
}

uint8_t IADXL3XX::getDeviceID(){
    return readRegister(ADXL3XX_REGISTERS::PARTID);
}

uint8_t IADXL3XX::getMaskRevision(){
    return readRegister(ADXL3XX_REGISTERS::REVID);
}


void IADXL3XX::writeRegister(const ADXL3XX_REGISTERS& reg, uint8_t value){
    Wire.beginTransmission(this->_i2caddr);
    Wire.write(static_cast<uint8_t>(reg));
    Wire.write(value);
    Wire.endTransmission();
}


uint8_t IADXL3XX::readRegister(const ADXL3XX_REGISTERS& reg){
    Wire.beginTransmission(this->_i2caddr);
    Wire.write(static_cast<uint8_t>(reg));
    Wire.endTransmission();
    Wire.beginTransmission(this->_i2caddr);
    Wire.requestFrom(this->_i2caddr, uint8_t(1));
    uint8_t val = Wire.read();

    Wire.endTransmission();    
    return val;
}


int32_t IADXL3XX::read20(const ADXL3XX_REGISTERS& reg) {
    Wire.beginTransmission(this->_i2caddr);
    Wire.write(static_cast<uint8_t>(reg));
    Wire.endTransmission();
    Wire.beginTransmission(this->_i2caddr);
    Wire.requestFrom(this->_i2caddr, uint8_t(3));
    uint8_t data3 = Wire.read();
    uint8_t data2 = Wire.read();
    uint8_t data1 = Wire.read();

    int32_t val = static_cast<int32_t>(data3) << 16;
    val |= (static_cast<int32_t>(data2) << 8);
    val |= (static_cast<int32_t>(data1) & 0xF0);

    val = (val<<8)>>12;

    Wire.endTransmission();
    return static_cast<int32_t>(val);
}

int32_t IADXL3XX::getX() { 
    return read20(ADXL3XX_REGISTERS::XDATA3); 
}

int32_t IADXL3XX::getY() {
    return read20(ADXL3XX_REGISTERS::YDATA3);
}

int32_t IADXL3XX::getZ() {
    return read20(ADXL3XX_REGISTERS::ZDATA3);
}


bool IADXL3XX::getXYZ(int32_t &x, int32_t &y, int32_t &z) {
    Wire.beginTransmission(this->_i2caddr);
    Wire.write(static_cast<uint8_t>(ADXL3XX_REGISTERS::XDATA3));
    Wire.endTransmission();
    Wire.beginTransmission(this->_i2caddr);
    Wire.requestFrom(this->_i2caddr, uint8_t(9));

    x = static_cast<int32_t>(Wire.read()) << 16;
    x |= (static_cast<int32_t>(Wire.read()) << 8);
    x |= (static_cast<int32_t>(Wire.read()) & 0xF0);
    x = (x<<8)>>12;

    y = static_cast<int32_t>(Wire.read()) << 16;
    y |= (static_cast<int32_t>(Wire.read()) << 8);
    y |= (static_cast<int32_t>(Wire.read()) & 0xF0);
    y = (y<<8)>>12;

    z = static_cast<int32_t>(Wire.read()) << 16;
    z |= (static_cast<int32_t>(Wire.read()) << 8);
    z |= (static_cast<int32_t>(Wire.read()) & 0xF0);
    z = (z<<8)>>12;

    Wire.endTransmission();

    return true;
}

uint8_t IADXL3XX::getFIFOEntries()
{
    uint8_t val = readRegister(ADXL3XX_REGISTERS::FIFO_ENTRIES);

    return val & 0b01111111;
}

bool IADXL3XX::getFIFOData(int32_t &x, int32_t &y, int32_t &z)
{
    Wire.beginTransmission(this->_i2caddr);
    Wire.write(static_cast<uint8_t>(ADXL3XX_REGISTERS::FIFO_DATA));
    Wire.endTransmission();
    Wire.requestFrom(this->_i2caddr, uint8_t(9));

    x = static_cast<int32_t>(Wire.read()) << 16;
    x |= (static_cast<int32_t>(Wire.read()) << 8);
    x |= (static_cast<int32_t>(Wire.read()) & 0xF0);
    x = (x<<8)>>12;

    y = static_cast<int32_t>(Wire.read()) << 16;
    y |= (static_cast<int32_t>(Wire.read()) << 8);
    y |= (static_cast<int32_t>(Wire.read()) & 0xF0);
    y = (y<<8)>>12;

    z = static_cast<int32_t>(Wire.read()) << 16;
    z |= (static_cast<int32_t>(Wire.read()) << 8);
    z |= (static_cast<int32_t>(Wire.read()) & 0xF0);
    z = (z<<8)>>12;

    return true;
}

int8_t IADXL3XX::getAllFIFOData(int32_t data[])
{
    int8_t n = this->getFIFOEntries();

    if(n%3){
        Wire.beginTransmission(this->_i2caddr);
        Wire.write(static_cast<uint8_t>(ADXL3XX_REGISTERS::FIFO_DATA));
        Wire.endTransmission();
        Wire.requestFrom(this->_i2caddr, uint8_t( (n%3) * 3));
        for(int i = (n%3) * 3; i > 0; --i){
            Wire.read();
        }
        n = n - n%3;
    }

    return this->getNFIFOData(n, data, data+n/3, data+n/3 * 2);
}

int8_t IADXL3XX::getAllFIFOData(int32_t x[], int32_t y[], int32_t z[])
{
    int8_t n = this->getFIFOEntries();

    if(n%3){
        Wire.beginTransmission(this->_i2caddr);
        Wire.write(static_cast<uint8_t>(ADXL3XX_REGISTERS::FIFO_DATA));
        Wire.endTransmission();
        Wire.requestFrom(this->_i2caddr, uint8_t( (n%3) * 3));
        for(int i = (n%3) * 3; i > 0; --i){
            Wire.read();
        }
        n = n - n%3;
    }

    return this->getNFIFOData(n, x, y, z);
}

int8_t IADXL3XX::getNFIFOData(int8_t n, int32_t x[], int32_t y[], int32_t z[])
{

    if(n > 3){
        if( n <= 9){
            Wire.beginTransmission(this->_i2caddr);
            Wire.write(static_cast<uint8_t>(ADXL3XX_REGISTERS::FIFO_DATA));
            Wire.endTransmission();
            Wire.requestFrom(this->_i2caddr, uint8_t(n * 3));

            for (size_t i = 0; i < n/3; i++)
            {
                Read20BitInt(x+i);
                Read20BitInt(y+i);
                Read20BitInt(z+i);
            }
            
            return n;
        }else{
            Wire.beginTransmission(this->_i2caddr);
            Wire.write(static_cast<uint8_t>(ADXL3XX_REGISTERS::FIFO_DATA));
            Wire.endTransmission();
            Wire.requestFrom(this->_i2caddr, uint8_t(9 * 3));

            Read20BitInt(x);
            Read20BitInt(y);
            Read20BitInt(z);

            Read20BitInt(x+1);
            Read20BitInt(y+1);
            Read20BitInt(z+1);

            Read20BitInt(x+2);
            Read20BitInt(y+2);
            Read20BitInt(z+2);
            return 9 + getNFIFOData(n-9, x+3, y+3, z+3);
        }

    }

    return 0;
}
