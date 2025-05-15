#pragma once
#include <sensor_packet.h>

constexpr int i2c_clock_speed = 400000;

constexpr int pressure_interrupt_pin = 9;
constexpr int imu_interrupt_pin = 6;

constexpr int sd_card_cs_pin = 10;
constexpr size_t packet_size_bytes = sizeof(SensorPacket);
constexpr int log_page_size_bytes = 1024;
constexpr int log_file_size = 1; // preallocated size in Mb
