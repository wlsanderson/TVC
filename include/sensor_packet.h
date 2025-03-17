#pragma once

struct SensorPacket {
    unsigned long timestamp;
    float pressure;
    float temperature;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float acc_x;
    float acc_y;
    float acc_z;
    
};