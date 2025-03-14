#pragma once
#include <stdint.h>

void initSPI();
uint8_t readRegister(uint8_t addr, uint8_t pin);
void readRegisters(uint8_t start_addr, uint8_t* buffer, int num_bytes, uint8_t pin);
void writeRegister(uint8_t addr, uint8_t value, uint8_t pin);

