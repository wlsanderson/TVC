#pragma once
#include <Arduino.h>
#include <cstdint>
#include <ArduinoEigen.h>
#include <lsm9ds1.h>
#include <tvc_utils.h>

using namespace Eigen;


template <size_t x_dim, size_t z_dim>
class QuatUKF {
public:
    QuatUKF(double alpha_, double beta_, double kappa_)
        : alpha{alpha_}, beta{beta_}, kappa{kappa_}
    {
        //compute_weights();
    }

    /**
     * This method should be changed if the intended number of states will be changed. Sets the
     * initial state estimate based on the inputted first packets. Not super accurate and a better
     * way would be to average the first 0.1 seconds or so of data
     */
    int begin(float pressure, int16_t ax, int16_t ay, int16_t az, int16_t mx, int16_t my, int16_t mz) {
        if (x_dim < 4 || x_dim > 50) {
            Serial.println("QuatUKF: x_dim missing or out of bounds!");
            return 1;
        }
        x(2) = (double)ax * acc_scale_factor;
        x(3) = (double)ay * acc_scale_factor;
        x(4) = (double)az * acc_scale_factor;
        x(8) = (double)mx * mag_scale_factor;
        x(9) = (double)my * mag_scale_factor;
        x(10) = (double)mz * mag_scale_factor;
        
        // getting the initial quaternions by converting the acc and mag to quaternion orientation.
        // this assumes that the imu is not moving on turn on.
        x(lastN(4)) = acc_mag_to_quat(x(seqN(2,3)), x(seqN(8,3)));
        p *= 0.01; // 0.01 initial P for all states
        p(lastN(3)) *= 10; // except for quaternions, initial P is 0.1
        return 0;
    }

    int predict();
    int update();
private:
    double alpha;
    double beta;
    double kappa;

    Vector<double, x_dim * 2 - 1> mean_w = Vector<double, x_dim * 2 - 1>::Identity();
    Vector<double, x_dim * 2 - 1> cov_w = Vector<double, x_dim * 2 - 1>::Identity();
    Vector<double, x_dim> x = Vector<double, x_dim>::Zero();
    Vector<double, x_dim - 1> p = Vector<double, x_dim - 1>::Identity();


    void compute_weights() {
        double lambda_ = alpha * alpha * (x_dim - 1 + kappa) - (x_dim - 1);
        double c = 0.5 / ((x_dim - 1) + lambda_);
        mean_w *= c;
        cov_w *= c;
        mean_w(0) = lambda_ / (x_dim - 1 + lambda_);
        cov_w(0) = lambda_ / (x_dim - 1 + lambda_) + (1 - alpha * alpha * beta);
    }

};