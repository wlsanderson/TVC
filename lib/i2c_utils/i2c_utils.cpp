#include "constants.h"

uint8_t i2c_read_register(uint8_t device_addr, uint8_t reg_addr) {
    Wire.beginTransmission(device_addr);
    Wire.write(reg_addr); 
    Wire.endTransmission(false);
    Wire.requestFrom(device_addr, 1);
    uint8_t val = Wire.read();
    return val;
}

void i2c_read_registers(uint8_t device_addr, uint8_t reg_addr, uint8_t* buffer, int num_bytes) {
    Wire.beginTransmission(device_addr);
    Wire.write(reg_addr); 
    Wire.endTransmission(false);
    Wire.requestFrom(device_addr, num_bytes);
    for (int i = 0; i < num_bytes; i++) {
        buffer[i] = Wire.read();
    }
}

void i2c_write_register(uint8_t device_addr, uint8_t reg_addr, uint8_t value) {
    Wire.beginTransmission(device_addr);
    Wire.write(reg_addr);
    Wire.write(value);
    Wire.endTransmission();
}