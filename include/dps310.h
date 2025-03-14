#include <stdint.h>

#pragma once

class DPS310 {
    public:
        void init();
        void fetch();
    private:
        int raw_temp {0};
        int raw_pressure {0};
        float calculate_pressure();
        float calculate_temp();
        void get_calibration_coefs();
        int32_t read_next();
        int16_t c0 {};
        int16_t c1 {};
        int32_t c10 {};
        int16_t c01 {};
        int32_t c00 {};
        int16_t c11 {};
        int16_t c20 {};
        int16_t c21 {};
        int16_t c30 {};
};