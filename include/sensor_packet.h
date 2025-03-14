#pragma once

struct SensorPacket {
    unsigned long timestamp {};
    float pressure {};
    float temperature {};
};