#pragma once
#include <Arduino.h>
#include <dps310.h>
#include <lsm9ds1.h>
#include <sensor_packet.h>

#include "constants.h"


class TVCContext {
    public:
        void init();
        void update();
        std::queue<SensorPacket> sensor_packet_queue; 
    private:
        DPS310 pressure_sensor;
        IMU imu;
};