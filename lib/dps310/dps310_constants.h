#pragma once

#include <stdint.h>
#include <spi_utils.h>


// DPS310 Registers
constexpr uint8_t read_byte = 0x80;
constexpr uint8_t PRS_CFG = 0x06;
constexpr uint8_t TMP_CFG = 0x07;
constexpr uint8_t ID = 0x0D;
constexpr uint8_t MEAS_CFG = 0x08;
constexpr uint8_t CFG_REG = 0x09; // FIFO and interrupt config

// DPS310 Constants
constexpr int spi_mode = SPI_MODE0;
constexpr BitOrder bit_order = MSBFIRST;
constexpr int spi_speed = 10000000;
constexpr int NUM_ADDR_COEFS = 18; // number of calibration coefficient addresses for DPS310 
constexpr int NUM_COEFS = 9; // number of calibration coefficients for DPS310
constexpr int kP = 524288;
constexpr int kT = 524288;
constexpr int dps310_sensor_buffer = 32; // the internal fifo stores 32 shared pressure/temp slots