#pragma once
#include <Arduino.h>
#include <dps310.h>
#include <lsm9ds1.h>
#include <sd_logger.h>
#include <sensor_packet.h>
#include <etl/queue.h>

#include "constants.h"


class TVCContext {
public:
    void init();
    void update();
private:
    DPS310 pressure_sensor;
    IMU imu;
    Logger logger;

    // double buffer for logging
    SensorPacket sensor_packet_buffer_1[log_page_size_bytes / packet_size_bytes];
    SensorPacket sensor_packet_buffer_2[log_page_size_bytes / packet_size_bytes];
    unsigned int next_packet_index = 0;
    bool filling_buffer_1 = true;
    bool ready_to_log = false;
    bool determine_buffer(int next_index);

    static TVCContext* instance;  // for interrupt handling
    static void pressure_interrupt_handler();  // static ISR proxy
    void handle_pressure_interrupt();  // sets data ready flag
    static void imu_interrupt_handler();  // static ISR proxy
    void handle_imu_interrupt();  // sets data ready flag
    volatile bool pressure_temp_ready = false;
    volatile bool imu_mag_ready = false;
    
};