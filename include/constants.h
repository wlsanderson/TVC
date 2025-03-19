#pragma once
#include <stdint.h>

#include <Wire.h>


// I2C
constexpr int i2c_speed = 100000;

// LSM9DS1 Registers
constexpr uint8_t imu_addr = 0x6B;
constexpr uint8_t mag_addr = 0x1E;
constexpr uint8_t CTRL_REG1_G = 0x10;
constexpr uint8_t STATUS_REG = 0x17;
constexpr uint8_t CTRL_REG9 = 0x23;
constexpr uint8_t CTRL_REG6_XL = 0x20;
constexpr uint8_t FIFO_CTRL = 0x2E;
constexpr uint8_t FIFO_SRC = 0x2F;
constexpr uint8_t dfgfdgF = FIFO_SRC + FIFO_CTRL;
constexpr uint8_t OUT_TEMP = 0x15; // start of burst read for temperature
constexpr uint8_t OUT_X_G = 0x18; // start of burst read for acc + gyro

constexpr uint8_t CTRL_REG1_M = 0x20;
constexpr uint8_t CTRL_REG3_M = 0x22;
constexpr uint8_t CTRL_REG4_M = 0x23;
constexpr uint8_t STATUS_REG_M = 0x27; 

// LSM9DS1 Constants
constexpr float gyro_scale_factor = 0.07; // at 2000 dps, 70 mdps/LSB
constexpr float acc_scale_factor = 0.000732; // at 16g, 0.732 mg/LSB
constexpr float mag_scale_factor = 0.00014; // at 4gauss, 0.14 mgauss/LSB