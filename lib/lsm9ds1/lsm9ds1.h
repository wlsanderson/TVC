#pragma once

#include <i2c_utils.h>
#include <sensor_packet.h>
#include <tvc_utils.h>

#include "lsm9ds1_constants.h"

class IMU {
public:
    void init();
    int fetch_imu_mag(SensorPacket* buffer, unsigned int buffer_index);        
private:
    I2CUtils i2c_imu;
    I2CUtils i2c_mag;

};