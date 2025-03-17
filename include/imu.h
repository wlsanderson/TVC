#pragma once
#include <queue>
#include "sensor_packet.h"

class IMU {
    public:
        void init();
        void fetch(std::queue<SensorPacket>&);
        void fetch_mag(std::queue<SensorPacket>&);
};