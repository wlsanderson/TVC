#pragma once
#include <stdint.h>

uint8_t spi_read_register(uint8_t addr, uint8_t pin);
void spi_read_registers(uint8_t start_addr, uint8_t* buffer, int num_bytes, uint8_t pin);
void spi_write_register(uint8_t addr, uint8_t value, uint8_t pin);

