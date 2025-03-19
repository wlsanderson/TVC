#pragma once
#include <queue>

#include <i2c_utils.h>
#include <sensor_packet.h>

class IMU {
    public:
        IMU(); // for normal use
        IMU(I2CUtils* i2c_utils_imu, I2CUtils* i2c_utils_mag); // injection for testing
        ~IMU();
        void init();
        void fetch_imu(std::queue<SensorPacket>&);
        void fetch_mag(std::queue<SensorPacket>&);
    
    private:
        I2CUtils* i2c_imu;
        I2CUtils* i2c_mag;
        bool owns_i2c; // indicates if IMU is responsible for deleting I2CUtils object

};