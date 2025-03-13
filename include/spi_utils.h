#pragma once
#include <stdint.h>

void initSPI();
uint8_t readRegister(uint8_t addr);
void readRegisters(uint8_t start_addr, uint8_t* buffer, int num_bytes);
void writeRegister(uint8_t addr, uint8_t value);

