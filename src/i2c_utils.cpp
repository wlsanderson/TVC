#include "constants.h"

uint8_t i2c_read_register(uint8_t device_addr, uint8_t reg_addr) {
    Wire.beginTransmission(device_addr);
    Wire.write(reg_addr); 
    Wire.requestFrom(device_addr, 1);
    uint8_t val = Wire.read();
    Wire.endTransmission();
    return val;
}

uint8_t i2c_write_register(uint8_t device_addr, uint8_t reg_addr, uint8_t value) {
    Wire.beginTransmission(device_addr);
    Wire.write(reg_addr);
    Wire.write(value);
    Wire.endTransmission();
}