#pragma once

#include <i2c_utils.h>
#include <sensor_packet.h>
#include <tvc_utils.h>

#include "lsm9ds1_constants.h"

class IMU {
public:
    IMU(uint8_t lsm9ds1_imu_address, uint8_t lsm9ds1_mag_address);
    bool init();
    size_t fetch_imu_mag(SensorPacket* buffer, size_t buffer_index);
    bool hold_interrupt = false;
private:
    uint8_t imu_address;
    uint8_t mag_address;
    I2CUtils i2c_imu;
    I2CUtils i2c_mag;
    int16_t imu_data[6];
    int16_t mag_data[3];
    bool startup();
    bool discard();
};