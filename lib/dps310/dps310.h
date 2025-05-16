#pragma once
#include <stdint.h>
#include <queue>

#include <i2c_utils.h>
#include <sensor_packet.h>
#include <tvc_utils.h>

#include "dps310_constants.h"

class DPS310 {
public:
    void init();
    int fetch(SensorPacket* buffer, unsigned int buffer_index);
private:
    I2CUtils i2c;
    
    float calculate_pressure();
    float calculate_temp();
    void get_calibration_coefs();
    uint8_t bytes[6];
    int raw_temp = 0.0;
    int raw_pressure = 0.0;
    int16_t c0;
    int16_t c1;
    int32_t c10;
    int16_t c01;
    int32_t c00;
    int16_t c11;
    int16_t c20;
    int16_t c21;
    int16_t c30;
};