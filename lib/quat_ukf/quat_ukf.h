#pragma once
#include <Arduino.h>
#include <stdint.h>


class QuatUKF {
public:
    QuatUKF(uint8_t num_states, float alpha, float beta, float kappa);
    bool predict();
    bool update();
private:
    uint8_t n;
    float alpha;
    float beta;
    float kappa;
    
};