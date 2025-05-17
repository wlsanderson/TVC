#pragma once
#include <sensor_packet.h>

// i2c
constexpr int i2c_clock_speed = 400000;

// interrupts
constexpr int pressure_interrupt_pin = 9;
constexpr int imu_interrupt_pin = 6;

// logger
constexpr int sd_card_cs_pin = 10;
constexpr size_t packet_size_bytes = sizeof(SensorPacket);
constexpr size_t log_page_size_bytes = 1024;
constexpr size_t log_file_size = 1; // preallocated size in Mb

// UKF
// STATES: altitude, vertical velocity, ax, ay, az, gx, gy, gz, mx, my, mz, qw, qx, qy, qz
// MEASUREMENTS: pressure, ax, ay, az, gx, gy, gz, mx, my, mz
constexpr size_t ukf_number_of_states =  15; // number of elements in UKF state vector
constexpr size_t ukf_number_of_measurements =  10; // number of elements in UKF measurement vector
constexpr float alpha_coefficient = 1;
constexpr float beta_coefficient = 2;
constexpr float kappa_coefficient = 0;