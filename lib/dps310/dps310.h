#pragma once
#include <stdint.h>
#include <queue>

#include <spi_utils.h>
#include <sensor_packet.h>

#include "dps310_constants.h"

class DPS310 {
    public:
        DPS310(int CS_pin); // for normal use
        DPS310(SPIUtils* spi_utils); // dependency injection for mock testing
        ~DPS310();
        void init();
        void fetch(std::queue<SensorPacket>&);

    private:
        SPIUtils* spi;
        bool owns_spi; // indicates if DPS310 is responsible for deleting SPIUtils object
        
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