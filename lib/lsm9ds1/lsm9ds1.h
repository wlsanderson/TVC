#pragma once

#include <i2c_utils.h>
#include <sensor_packet.h>
#include <tvc_utils.h>

#include "lsm9ds1_constants.h"

class IMU {
public:
    bool init();
    int fetch_imu_mag(SensorPacket* buffer, unsigned int buffer_index);        
private:
    I2CUtils i2c_imu;
    I2CUtils i2c_mag;
    int16_t imu_data[6];
    int16_t mag_data[3];
    bool startup();
    bool discard();
};