#pragma once
#include <stdint.h>
#include <queue>

#include <i2c_utils.h>
#include <sensor_packet.h>
#include <tvc_utils.h>

#include "dps310_constants.h"

class DPS310 {
public:
    void init();
    template<typename QueueType>
    void fetch(QueueType& packet_queue) {
        
        // checks if pressure and temp measurements are available
        uint8_t data_ready = i2c.read_register(MEAS_CFG);
        
        // resets the interrupt pins, must be read so that interrupt pin will return to high
        i2c.read_register(INT_STS);

        SensorPacket packet;
        packet.timestamp = micros();
        
        // bits 4 and 5 are pressure ready and temp ready, respectively
        if ((data_ready & 0x30) == 0x30) {
            // pressure measurement
            uint8_t bytes[3];
            i2c.read_registers(0x00, bytes, 3);
            raw_pressure = twos_complement_24(bytes[0], bytes[1], bytes[2]);
            packet.pressure = calculate_pressure();

            // temperature measurement
            i2c.read_registers(0x03, bytes, 3);
            raw_temp = twos_complement_24(bytes[0], bytes[1], bytes[2]);
            packet.temperature = calculate_temp();
            
            if (!packet_queue.full()) {
                packet_queue.push(packet);
            }
        }
    }

private:
    I2CUtils i2c;
    
    float calculate_pressure();
    float calculate_temp();
    void get_calibration_coefs();

    int raw_temp = 0.0;
    int raw_pressure = 0.0;
    int16_t c0;
    int16_t c1;
    int32_t c10;
    int16_t c01;
    int32_t c00;
    int16_t c11;
    int16_t c20;
    int16_t c21;
    int16_t c30;
};