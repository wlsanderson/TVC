#include "constants.h"
#include "imu.h"
#include "i2c_utils.h"
#include "sensor_packet.h"

void IMU::init() {
    //i2c_write_register(imu_addr, 0x22, 0b00000101);
    

    Serial.print("LSM9DS1 IMU: ");
    Serial.println(i2c_read_register(imu_addr, 0x0F), BIN);

    Serial.print("LSM9DS1 MAG: ");
    Serial.println(i2c_read_register(mag_addr, 0x0F), BIN);
    /*
    ACC/GYRO:

    0
    SLEEP_G: sleep mode enabled: 0
    0
    FIFO_TEMP_EN: temperature data stored in FIFO: 0
    DRDY_mask_bit: data available enable bit: 0
    I2C_DISABLE: disables I2C: 0 
    FIFO_EN: enables FIFO: 1
    STOP_ON_FTH: Enables FIFO threshold usage: 0
    */
    i2c_write_register(imu_addr, CTRL_REG9, 0b00000010);

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

    i2c_write_register(imu_addr, CTRL_REG1_G, 0b00111011); // sets gyro ODR
    i2c_write_register(imu_addr, CTRL_REG6_XL, 0b00101000); // sets accel ODR, 16g cap
    i2c_write_register(imu_addr, FIFO_CTRL, 0b01100000); // FIFO continuous mode
    i2c_write_register(mag_addr, CTRL_REG1_M, 0b11001100); // 5hz Mag ODR, High-performance mode
    i2c_write_register(mag_addr, CTRL_REG4_M, 0b00001000); // High-performance mode

}



void IMU::fetch(std::queue<SensorPacket>& packet_queue) {
    // bits [5:0] of FIFO_SRC represent number of unread samples
    uint8_t unread_samples = (i2c_read_register(imu_addr, FIFO_SRC)) & 0x3F;
    while (unread_samples > 1) { // checking for 2 or more because gyro + acc = 2 
        SensorPacket packet;
        packet.timestamp = micros();
        // read gyro and accel
        uint8_t bytes[12];
        i2c_read_registers(imu_addr, OUT_X_G, bytes, 12);
        int16_t gx = (int16_t)((bytes[1] << 8) | bytes[0]);
        int16_t gy = (int16_t)((bytes[3] << 8) | bytes[2]);
        int16_t gz = (int16_t)((bytes[5] << 8) | bytes[4]);
        int16_t ax = (int16_t)((bytes[7] << 8) | bytes[6]);
        int16_t ay = (int16_t)((bytes[9] << 8) | bytes[8]);
        int16_t az = (int16_t)((bytes[11] << 8) | bytes[10]);

        packet.gyro_x = (float)(gx) * gyro_scale_factor;
        packet.gyro_y = (float)(gy) * gyro_scale_factor;
        packet.gyro_z = (float)(gz) * gyro_scale_factor;
        packet.acc_x = (float)(ax) * acc_scale_factor;
        packet.acc_y = (float)(ay) * acc_scale_factor;
        packet.acc_z = (float)(az) * acc_scale_factor;
        packet_queue.push(packet);
        unread_samples = (i2c_read_register(imu_addr, FIFO_SRC)) & 0x3F;
    }   
}

void IMU::fetch_mag(std::queue<SensorPacket>& packet_queue) {
    bool data_available = i2c_read_register(mag_addr, 0x27) & 0x08;
    while (data_available) {
        SensorPacket packet;
        packet.timestamp = micros();
        uint8_t bytes[6];
        i2c_read_registers(mag_addr, 0x28, bytes, 6);
        int16_t mag_x = (int16_t)((bytes[1] << 8) | bytes[0]);
        int16_t mag_y = (int16_t)((bytes[3] << 8) | bytes[2]);
        int16_t mag_z = (int16_t)((bytes[5] << 8) | bytes[4]);

        packet.mag_x = (float)(mag_x) * mag_scale_factor;
        packet.mag_y = (float)(mag_y) * mag_scale_factor;
        packet.mag_z = (float)(mag_z) * mag_scale_factor;

        packet_queue.push(packet);

        data_available = i2c_read_register(mag_addr, 0x27) & 0x08;
    }
}