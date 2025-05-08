#pragma once
#include <Arduino.h>
#include <dps310.h>
#include <lsm9ds1.h>
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
    etl::queue<SensorPacket, SENSOR_QUEUE_SIZE> sensor_packet_queue;
    static TVCContext* instance;  // for interrupt handling
    static void pressure_interrupt_handler();  // static ISR proxy
    void handle_pressure_interrupt();  // sets data ready flag
    static void imu_interrupt_handler();  // static ISR proxy
    void handle_imu_interrupt();  // sets data ready flag
    volatile bool pressure_temp_ready = false;
    volatile bool imu_mag_ready = false;
};