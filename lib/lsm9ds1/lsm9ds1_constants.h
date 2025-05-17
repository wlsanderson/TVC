#pragma once
#include <stdint.h>


/* ODR selection of IMU */
enum IMU_ODR {
    IMU_PD = 0,
    IMU_14Hz9 = 1, // 14.9hz
    IMU_59Hz5 = 2, // 59.5hz in datasheet
    IMU_119Hz = 3,
    IMU_238Hz = 4,
    IMU_476Hz = 5,
    IMU_952Hz = 6,
};

/* FIFO mode selection */
enum IMU_FIFO_MODE {
    BYPASS_MODE = 0,
    FIFO_MODE = 1,
    CONTINUOUS_TO_FIFO_MODE = 3,
    BYPASS_TO_CONTINUOUS_MODE = 4,
    CONTINUOUS_MODE = 6,
};

/* Gyroscope full scale ranges */
enum GYRO_RANGES {
    DPS_245 = 0,
    DPS_500 = 1,
    DPS_2000 = 3,
};

/* Accelerometer full scale ranges */
enum ACC_RANGES {
    GS_2 = 0,
    GS_16 = 1,
    GS_4 = 2,
    GS_8 = 3,
};

/* Magnetometer full scale ranges */
enum MAG_RANGES {
    GAUSS_4 = 0,
    GAUSS_8 = 1,
    GAUSS_12 = 2,
    GAUSS_16 = 3,
};

/* Accelerometer bandwidth selection */
enum ACC_BANDWIDTH {
    AUTO = 0,
    BW_408 = 0x04,
    BW_211 = 0x05,
    BW_105 = 0x06,
    BW_50 = 0x07,
};

/* Accelerometer Low pass filter cutoff frequency */
enum ACC_LP_FILTER {
    LP_DISABLE = 0x00,
    LP_ODR_DIV_50 = 0x04,
    LP_ODR_DIV_100 = 0x05,
    LP_ODR_DIV_9 = 0x06,
    LP_ODR_DIV_400 = 0x07,
};

/* LPF Filter for gyroscope*/
enum GYRO_LP_FILTER {
    G_LP_STRONG = 0,
    G_LP_MEDIUM = 1,
    G_LP_LIGHT = 2,
    G_LP_ULTRALIGHT = 3,
};

/* Magnetometer ODRs. Uses Low-Performance unless 155Hz, 300Hz, or 560hz */
enum MAG_ODR {
    MAG_0Hz625 = 0, // 0.625Hz
    MAG_1Hz25 = 0x02, // 1.25Hz
    MAG_2Hz5 = 0x04, // 2.5Hz
    MAG_5Hz = 0x06,
    MAG_10Hz = 0x08,
    MAG_20Hz = 0x0A,
    MAG_40Hz = 0x0C,
    MAG_80Hz = 0x0E,
    MAG_155Hz = 0x31,
    MAG_300Hz = 0x21,
    MAG_560Hz = 0x11,
    MAG_1000Hz = 0x01,
};

// LSM9DS1 Registers
constexpr uint8_t WHO_AM_I = 0x0F;
constexpr uint8_t INT1_CTRL = 0x0C; // interrupt pin 1 config
constexpr uint8_t CTRL_REG1_G = 0x10;
constexpr uint8_t CTRL_REG2_G = 0x11;
constexpr uint8_t CTRL_REG3_G = 0x12;
constexpr uint8_t STATUS_REG = 0x17;
constexpr uint8_t CTRL_REG6_XL = 0x20;
constexpr uint8_t CTRL_REG7_XL = 0x21;
constexpr uint8_t CTRL_REG8 = 0x22;
constexpr uint8_t CTRL_REG9 = 0x23;
constexpr uint8_t FIFO_CTRL = 0x2E;
constexpr uint8_t FIFO_SRC = 0x2F;
constexpr uint8_t OUT_TEMP = 0x15; // start of burst read for temperature
constexpr uint8_t OUT_X_G = 0x18; // start of burst read for acc + gyro

constexpr uint8_t WHO_AM_I_M = 0x0F;
constexpr uint8_t CTRL_REG1_M = 0x20;
constexpr uint8_t CTRL_REG2_M = 0x21;
constexpr uint8_t CTRL_REG3_M = 0x22;
constexpr uint8_t CTRL_REG4_M = 0x23;
constexpr uint8_t CTRL_REG5_M = 0x24;
constexpr uint8_t STATUS_REG_M = 0x27; 
constexpr uint8_t OUT_X_L_M = 0x28; // start of burst read for magnetometer

// LSM9DS1 Constants
constexpr int lsm9ds1_i2c_speed = 400000;
constexpr float gyro_scale_factor = 0.07; // at 2000 dps, 70 mdps/LSB
constexpr float acc_scale_factor = 0.000732; // at 16g, 0.732 mg/LSB
constexpr float mag_scale_factor = 0.014; // at 4gauss, 0.14 mgauss/LSB = 0.014 micro tesla/LSB