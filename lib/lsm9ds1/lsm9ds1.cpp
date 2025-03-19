#include <lsm9ds1.h>
#include <tvc_utils.h>

#include "lsm9ds1_constants.h"


// constructor for normal use
IMU::IMU() {
    i2c_imu = new I2CUtils(imu_addr, i2c_speed);
    i2c_mag = new I2CUtils(mag_addr, i2c_speed);
    owns_i2c = true;
}

// constructor for dependency injection
IMU::IMU(I2CUtils* i2c_utils_imu, I2CUtils* i2c_utils_mag) {
    i2c_imu = i2c_utils_imu;
    i2c_mag = i2c_utils_mag;
    owns_i2c = false;
}

void IMU::init() {
    //i2c_write_register(imu_addr, 0x22, 0b00000101);
    

    Serial.print("LSM9DS1 IMU: ");
    Serial.println(i2c_imu->read_register(0x0F), BIN);

    Serial.print("LSM9DS1 MAG: ");
    Serial.println(i2c_mag->read_register(0x0F), BIN);
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
    i2c_imu->write_register(CTRL_REG9, 0b00000010);

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
    i2c_mag->write_register(CTRL_REG3_M, 0b00000000);

    i2c_imu->write_register(CTRL_REG1_G, 0b00111011); // sets gyro ODR
    i2c_imu->write_register(CTRL_REG6_XL, 0b00101000); // sets accel ODR, 16g cap
    i2c_imu->write_register(FIFO_CTRL, 0b01100000); // FIFO continuous mode
    i2c_mag->write_register(CTRL_REG1_M, 0b11001100); // 5hz Mag ODR, High-performance mode
    i2c_mag->write_register(CTRL_REG4_M, 0b00001000); // High-performance mode

}



void IMU::fetch_imu(std::queue<SensorPacket>& packet_queue) {
    // bits [5:0] of FIFO_SRC represent number of unread samples
    uint8_t unread_samples = (i2c_imu->read_register(FIFO_SRC)) & 0x3F;
    while (unread_samples > 1) { // checking for 2 or more because gyro + acc = 2 
        SensorPacket packet;
        packet.timestamp = micros();
        // read gyro and accel
        uint8_t bytes[12];
        i2c_imu->read_registers(OUT_X_G, bytes, 12);
        int16_t gx = twos_complement_16(bytes[1], bytes[0]);
        int16_t gy = twos_complement_16(bytes[3], bytes[2]);
        int16_t gz = twos_complement_16(bytes[5], bytes[4]);
        int16_t ax = twos_complement_16(bytes[7], bytes[6]);
        int16_t ay = twos_complement_16(bytes[9], bytes[8]);
        int16_t az = twos_complement_16(bytes[11], bytes[10]);

        packet.gyro_x = (float)(gx) * gyro_scale_factor;
        packet.gyro_y = (float)(gy) * gyro_scale_factor;
        packet.gyro_z = (float)(gz) * gyro_scale_factor;
        packet.acc_x = (float)(ax) * acc_scale_factor;
        packet.acc_y = (float)(ay) * acc_scale_factor;
        packet.acc_z = (float)(az) * acc_scale_factor;
        packet_queue.push(packet);
        unread_samples = (i2c_imu->read_register(FIFO_SRC)) & 0x3F;
    }   
}

void IMU::fetch_mag(std::queue<SensorPacket>& packet_queue) {
    bool data_available = i2c_mag->read_register(0x27) & 0x08;
    while (data_available) {
        SensorPacket packet;
        packet.timestamp = micros();
        uint8_t bytes[6];
        i2c_mag->read_registers(0x28, bytes, 6);
        int16_t mag_x = twos_complement_16(bytes[1], bytes[0]);
        int16_t mag_y = twos_complement_16(bytes[3], bytes[2]);
        int16_t mag_z = twos_complement_16(bytes[5], bytes[4]);

        packet.mag_x = (float)(mag_x) * mag_scale_factor;
        packet.mag_y = (float)(mag_y) * mag_scale_factor;
        packet.mag_z = (float)(mag_z) * mag_scale_factor;

        packet_queue.push(packet);

        data_available = i2c_mag->read_register(0x27) & 0x08;
    }
}

IMU::~IMU() {
    if (owns_i2c) {
        delete i2c_imu;
        delete i2c_mag;
    }
}