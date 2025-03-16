#pragma once
#include <stdint.h>
#include <SPI.h>
#include <Wire.h>

// Pins
const int CS_PIN_DPS310 = 10;

// SPI
const int spi_speed = 10000000;

// I2C
const int i2c_speed = 400000;

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

// LSM9DS1 Registers
const uint8_t imu_addr = 0x6B;
const uint8_t mag_addr = 0x1E;
const uint8_t CTRL_REG1_G = 0x10;
const uint8_t STATUS_REG = 0x17;
const uint8_t CTRL_REG9 = 0x23;
const uint8_t CTRL_REG6_XL = 0x1F;
const uint8_t FIFO_CTRL = 0x2E;

const uint8_t CTRL_REG1_M = 0x20;
const uint8_t CTRL_REG3_M = 0x22;
const uint8_t CTRL_REG4_M = 0x23;
const uint8_t STATUS_REG_M = 0x27; 

