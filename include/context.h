#pragma once
#include "dps310.h"
#include "imu.h"
#include "sensor_packet.h"

class TVCContext {
    public:
        void init();
        void update();
        std::queue<SensorPacket> sensor_packet_queue; 
    private:
        DPS310 pressure_sensor;
        IMU imu;
};