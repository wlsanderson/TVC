#include <i2c_utils.h>

void I2CUtils::begin(uint8_t i2c_device_addr) {
    this->device_addr = i2c_device_addr;
}

uint8_t I2CUtils::read_register(uint8_t reg_addr) {
    Wire.beginTransmission(device_addr);
    Wire.write(reg_addr); 
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)device_addr, (uint8_t)1);
    uint8_t val = Wire.read();
    return val;
}

void I2CUtils::read_registers(uint8_t reg_addr, uint8_t* buffer, size_t num_bytes) {
    Wire.beginTransmission(device_addr);
    Wire.write(reg_addr); 
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)device_addr, (uint8_t)num_bytes);
    for (size_t i = 0; i < num_bytes; i++) {
        buffer[i] = Wire.read();
    }
}

void I2CUtils::write_register(uint8_t reg_addr, uint8_t value) {
    Wire.beginTransmission(device_addr);
    Wire.write(reg_addr);
    Wire.write(value);
    Wire.endTransmission();
}