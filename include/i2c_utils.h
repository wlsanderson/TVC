#pragma once
#include <stdint.h>

uint8_t i2c_read_register(uint8_t device_addr, uint8_t reg_addr);
uint8_t i2c_write_register(uint8_t device_addr, uint8_t reg_addr, uint8_t value);