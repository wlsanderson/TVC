#pragma once

struct DPS310Packet {
    unsigned int delta_time {};
    float pressure {};
    float temperature {};
};