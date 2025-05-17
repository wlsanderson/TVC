#pragma once
#include <Arduino.h>
#include <cstdint>
#include <algorithm>
#include <ArduinoEigen.h>

template <size_t x_dim, size_t z_dim>
class QuatUKF {
public:
    QuatUKF(float alpha_, float beta_, float kappa_)
        : alpha{alpha_}, beta{beta_}, kappa{kappa_}
    {
        compute_weights();
    }
    int begin(float pressure, int16_t ax, int16_t ay, int16_t az, int16_t mx, int16_t my, int16_t mz) {
        if (x_dim < 4 || x_dim > 50) {
            Serial.println("QuatUKF: x_dim missing or out of bounds!");
            return 1;
        }
        x[2] = ax;
        x[3] = ay;
        x[4] = az;
        x[8] = mx;
        x[9] = my;
        x[10] = mz;
        
        return 0;
    }

    int predict();
    int update();
private:
    float alpha;
    float beta;
    float kappa;
    float mean_w[x_dim * 2 - 1];
    float cov_w[x_dim * 2 - 1];
    float x[x_dim];

    void compute_weights() {
        float lambda_ = alpha * alpha * (x_dim - 1 + kappa) - (x_dim - 1);
        float c = 0.5 / ((x_dim - 1) + lambda_);
        std::fill_n(mean_w, x_dim, c);
        std::fill_n(cov_w, x_dim, c);
        mean_w[0] = lambda_ / (x_dim - 1 + lambda_);
        cov_w[0] = lambda_ / (x_dim - 1 + lambda_) + (1 - alpha * alpha * beta);
    }

};