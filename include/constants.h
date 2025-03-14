#pragma once
#include <stdint.h>
#include <SPI.h>

// Pins
const int CS_PIN_DPS310 = 10;

// SPI
const int SPI_SPEED = 10000000;

// DPS310 Registers
const uint8_t READ = 0x80;
const uint8_t PRS_CFG = 0x06;
const uint8_t TMP_CFG = 0x07;
const uint8_t ID = 0x0D;
const uint8_t MEAS_CFG = 0x08;
const uint8_t CFG_REG = 0x09; // FIFO and interrupt config
const uint8_t FIFO_STS = 0x0B;

// DPS310 Constants
const int NUM_ADDR_COEFS = 18; // number of calibration coefficient addresses for DPS310 
const int NUM_COEFS = 9; // number of calibration coefficients for DPS310
const int kP = 524288;
const int kT = 524288;
const int dps310_sensor_buffer = 32; // the internal fifo stores 32 shared pressure/temp slots