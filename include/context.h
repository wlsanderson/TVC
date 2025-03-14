#pragma once
#include "dps310.h"

class TVCContext {
    public:
        void init();
        void update();
    private:
        DPS310 pressure_sensor {};
};