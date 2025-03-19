#pragma once
#include <stdint.h>

uint8_t i2c_read_register(uint8_t device_addr, uint8_t reg_addr);
void i2c_write_register(uint8_t device_addr, uint8_t reg_addr, uint8_t value);
void i2c_read_registers(uint8_t device_addr, uint8_t reg_addr, uint8_t* buffer, int num_bytes);