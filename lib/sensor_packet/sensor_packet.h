#pragma once

struct SensorPacket {
    unsigned long timestamp = 0.0;
    float pressure = 0.0;
    float temperature = 0.0;
    float gyro_x = 0.0;
    float gyro_y = 0.0;
    float gyro_z = 0.0;
    float acc_x = 0.0;
    float acc_y = 0.0;
    float acc_z = 0.0;
    float mag_x = 0.0;
    float mag_y = 0.0;
    float mag_z = 0.0;
};