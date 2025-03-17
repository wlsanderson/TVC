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

void IMU::fetch(std::queue<SensorPacket>& packet_queue) {
    // read temp
    uint8_t bytes_t[2];
    i2c_read_registers(imu_addr, OUT_TEMP, bytes_t, 2);
    int16_t temp = (int16_t)(bytes_t[1] << 8) | bytes_t[0];
    
    // read gyro
    uint8_t bytes_g[6]; 
    // i2c_read_registers(imu_addr, OUT_X_G, bytes_g, 6);
    int16_t gx = (int16_t)(bytes_g[1] << 8) | bytes_g[0];
    int16_t gy = (int16_t)(bytes_g[3] << 8) | bytes_g[2];
    int16_t gz = (int16_t)(bytes_g[5] << 8) | bytes_g[4];

    // read accel
    uint8_t bytes_a[6];
    // i2c_read_registers(imu_addr, 0x28, bytes_a, 6);
    int16_t ax = (int16_t)(bytes_a[1] << 8) | bytes_a[0];
    int16_t ay = (int16_t)(bytes_a[3] << 8) | bytes_a[2];
    int16_t az = (int16_t)(bytes_a[5] << 8) | bytes_a[4];

    
    Serial.println(temp);
    // Serial.println((float)(gx)*gyro_scale_factor);
    // Serial.println((float)(gy)*gyro_scale_factor);
    // Serial.println((float)(gz)*gyro_scale_factor);
    // Serial.println((float)(ax)*acc_scale_factor);
    // Serial.println((float)(ay)*acc_scale_factor);
    // Serial.println((float)(az)*acc_scale_factor);
}