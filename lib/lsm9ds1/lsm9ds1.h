#pragma once

#include <i2c_utils.h>
#include <sensor_packet.h>
#include <tvc_utils.h>

#include "lsm9ds1_constants.h"

class IMU {
public:
    void init();
    template<typename QueueType>
    void fetch_imu_mag(QueueType& packet_queue) {
        // bits 0 and 1 are new accelerometer data and new gyro data, respectively
        uint8_t imu_data_ready = i2c_imu.read_register(STATUS_REG);

        SensorPacket packet;
        bool has_data = false;
        if ((imu_data_ready & 0x03) == 0x03) {
            // fetching both accel and gyro
            packet.timestamp = micros();
            // read gyro and accel
            uint8_t bytes[12];
            i2c_imu.read_registers(OUT_X_G, bytes, 12);
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
            has_data = true;
        }
        uint8_t mag_data_ready = i2c_mag.read_register(STATUS_REG_M);
        // bit 3 is 1 if X, Y, and Z magnetometer data is available
        if ((mag_data_ready & 0x08) == 0x08) {
            uint8_t bytes[6];
            i2c_mag.read_registers(OUT_X_L_M, bytes, 6);
            int16_t mag_x = twos_complement_16(bytes[1], bytes[0]);
            int16_t mag_y = twos_complement_16(bytes[3], bytes[2]);
            int16_t mag_z = twos_complement_16(bytes[5], bytes[4]);
    
            packet.mag_x = (float)(mag_x) * mag_scale_factor;
            packet.mag_y = (float)(mag_y) * mag_scale_factor;
            packet.mag_z = (float)(mag_z) * mag_scale_factor;
            has_data = true;
        }
        if (has_data && !packet_queue.full()) {
            packet_queue.push(packet);
        }
    }
        
private:
    I2CUtils i2c_imu;
    I2CUtils i2c_mag;

};