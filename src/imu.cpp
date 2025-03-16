#include "constants.h"
#include "imu.h"
#include "i2c_utils.h"

void IMU::init() {
    /*
    ACC/GYRO:

    0
    SLEEP_G: sleep mode enabled: 0
    0
    FIFO_TEMP_EN: temperature data stored in FIFO: 1
    DRDY_mask_bit: data available enable bit: 0
    I2C_DISABLE: disables I2C: 0 
    FIFO_EN: enables FIFO: 1
    STOP_ON_FTH: Enables FIFO threshold usage: 0
    */
    i2c_write_register(imu_addr, CTRL_REG9, 0b00010010);

    /*
    MAG

    I2C_DISABLE: disables I2C: 0
    0
    LP: enables low-power mode: 0
    0
    0
    SIM: enables SPI: 1
    MD1/MD0: Operating Mode: 00 (continuous-conversion)
    */
    i2c_write_register(mag_addr, CTRL_REG3_M, 0b00000000);


    Serial.print("LSM9DS1: ");
    Serial.println(i2c_read_register(imu_addr, 0x0F), BIN);

    i2c_write_register(imu_addr, CTRL_REG1_G, 0b00111011); // sets gyro ODR
    i2c_write_register(imu_addr, CTRL_REG6_XL, 0b00101000); // sets accel ODR, 16g cap
    i2c_write_register(imu_addr, FIFO_CTRL, 0b01100000); // FIFO continuous mode
    i2c_write_register(mag_addr, CTRL_REG1_M, 0b11001100); // 5hz Mag ODR, High-performance mode
    i2c_write_register(mag_addr, CTRL_REG4_M, 0b00001000); // High-performance mode

}