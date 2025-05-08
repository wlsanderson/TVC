#pragma once
#include <stdint.h>

// LSM9DS1 Registers
constexpr uint8_t imu_addr = 0x6B; // i2c address
constexpr uint8_t WHO_AM_I = 0x0F;
constexpr uint8_t INT1_CTRL = 0x06; // interrupt pin 1 config
constexpr uint8_t CTRL_REG1_G = 0x10;
constexpr uint8_t CTRL_REG2_G = 0x11;
constexpr uint8_t STATUS_REG = 0x17;
constexpr uint8_t CTRL_REG8 = 0x22;
constexpr uint8_t CTRL_REG9 = 0x23;
constexpr uint8_t CTRL_REG6_XL = 0x20;
constexpr uint8_t FIFO_CTRL = 0x2E;
constexpr uint8_t FIFO_SRC = 0x2F;
constexpr uint8_t OUT_TEMP = 0x15; // start of burst read for temperature
constexpr uint8_t OUT_X_G = 0x18; // start of burst read for acc + gyro

constexpr uint8_t mag_addr = 0x1E; // i2c address
constexpr uint8_t WHO_AM_I_M = 0x0F;
constexpr uint8_t CTRL_REG1_M = 0x20;
constexpr uint8_t CTRL_REG2_M = 0x21;
constexpr uint8_t CTRL_REG3_M = 0x22;
constexpr uint8_t CTRL_REG4_M = 0x23;
constexpr uint8_t STATUS_REG_M = 0x27; 
constexpr uint8_t OUT_X_L_M = 0x28; // start of burst read for magnetometer

// LSM9DS1 Constants
constexpr int lsm9ds1_i2c_speed = 100000;
constexpr float gyro_scale_factor = 0.07; // at 2000 dps, 70 mdps/LSB
constexpr float acc_scale_factor = 0.000732; // at 16g, 0.732 mg/LSB
constexpr float mag_scale_factor = 0.014; // at 4gauss, 0.14 mgauss/LSB = 0.014 micro tesla/LSB