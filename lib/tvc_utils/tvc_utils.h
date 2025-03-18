#pragma once
#include <stdint.h>

int16_t twos_complement_12_hi(uint8_t msb_8, uint8_t lsb_4);
int16_t twos_complement_12_lo(uint8_t msb_4, uint8_t lsb_8);

int16_t twos_complement_16(uint8_t msb, uint8_t lsb);

int32_t twos_complement_20_hi(uint8_t msb_8, uint8_t lsb_8, uint8_t xlsb_4);
int32_t twos_complement_20_lo(uint8_t msb_4, uint8_t lsb_8, uint8_t xlsb_8);

int32_t twos_complement_24(uint8_t msb, uint8_t lsb, uint8_t xlsb);
