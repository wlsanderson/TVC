#pragma once
#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>

class I2CUtils {
    public:
        uint8_t device_addr = 0x00;
        int i2c_speed;

        void init(uint8_t device_addr, int i2c_speed);
        uint8_t read_register(uint8_t reg_addr);
        void write_register(uint8_t reg_addr, uint8_t value);
        void read_registers(uint8_t reg_addr, uint8_t* buffer, size_t num_bytes);
};


