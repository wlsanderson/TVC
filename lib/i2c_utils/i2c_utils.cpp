#include <i2c_utils.h>

void I2CUtils::init(uint8_t i2c_device_addr, int i2c_speed_hz) {
    this->device_addr = i2c_device_addr;
    this->i2c_speed = i2c_speed_hz;
}

uint8_t I2CUtils::read_register(uint8_t reg_addr) {
    Wire.beginTransmission(device_addr);
    Wire.write(reg_addr); 
    Wire.endTransmission(false);
    Wire.requestFrom(device_addr, 1);
    uint8_t val = Wire.read();
    return val;
}

void I2CUtils::read_registers(uint8_t reg_addr, uint8_t* buffer, int num_bytes) {
    Wire.beginTransmission(device_addr);
    Wire.write(reg_addr); 
    Wire.endTransmission(false);
    Wire.requestFrom(device_addr, num_bytes);
    for (int i = 0; i < num_bytes; i++) {
        buffer[i] = Wire.read();
    }
}

void I2CUtils::write_register(uint8_t reg_addr, uint8_t value) {
    Wire.beginTransmission(device_addr);
    Wire.write(reg_addr);
    Wire.write(value);
    Wire.endTransmission();
}