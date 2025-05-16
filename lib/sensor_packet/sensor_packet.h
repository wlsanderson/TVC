#pragma once

struct SensorPacket {
    unsigned long timestamp = 0;
    float pressure = 0.0;
    float temperature = 0.0;
    uint16_t id = 0;
    int16_t gyro_x = 0;
    int16_t gyro_y = 0;
    int16_t gyro_z = 0;
    int16_t acc_x = 0;
    int16_t acc_y = 0;
    int16_t acc_z = 0;
    int16_t mag_x = 0;
    int16_t mag_y = 0;
    int16_t mag_z = 0;
    
};