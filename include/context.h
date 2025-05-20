#pragma once
#include <Arduino.h>
#include <dps310.h>
#include <lsm9ds1.h>
#include <sd_logger.h>
#include <quat_ukf.h>
#include <sensor_packet.h>

#include "constants.h"


class TVCContext {
public:
    TVCContext();
    int begin();
    void update();
private:
    DPS310 pressure_sensor;
    IMU imu;
    Logger logger;
    QuatUKF<ukf_number_of_states, ukf_number_of_measurements> ukf;

    int first_fetch();
    int query_sensors();
    SensorPacket first_fetch_buffer[2];

    // double buffer for logging
    SensorPacket sensor_packet_buffer_1[log_page_size_bytes / packet_size_bytes];
    SensorPacket sensor_packet_buffer_2[log_page_size_bytes / packet_size_bytes];
    size_t next_packet_index = 0;
    bool filling_buffer_1 = true;
    bool ready_to_log = false;
    bool determine_buffer(size_t next_index);
    SensorPacket packet_to_process;

    static TVCContext* instance;  // for interrupt handling
    static void pressure_interrupt_handler();  // static ISR proxy
    void handle_pressure_interrupt();  // sets data ready flag
    static void imu_interrupt_handler();  // static ISR proxy
    void handle_imu_interrupt();  // sets data ready flag
    volatile bool pressure_temp_ready = false;
    volatile bool imu_mag_ready = false;
    void combine_packet(SensorPacket* buffer);
    
};