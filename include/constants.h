#pragma once
#include <sensor_packet.h>

constexpr size_t packet_size_bytes = sizeof(SensorPacket);

constexpr int pressure_interrupt_pin = 9;
constexpr int imu_interrupt_pin = 6;

constexpr int sd_card_cs_pin = 10;
constexpr int log_page_size_bytes = 2048;
constexpr int log_file_size = 1; // preallocated size in Mb
