#pragma once
#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>

class I2CUtils {
    public:
        uint8_t device_addr = 0x00;
        int i2c_speed;

        I2CUtils(uint8_t device_addr, int i2c_speed);
        virtual uint8_t read_register(uint8_t reg_addr);
        virtual void write_register(uint8_t reg_addr, uint8_t value);
        virtual void read_registers(uint8_t reg_addr, uint8_t* buffer, int num_bytes);
};


