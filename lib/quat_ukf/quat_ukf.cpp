#include <quat_ukf.h>

QuatUKF::QuatUKF(uint8_t num_states, float alpha, float beta, float kappa)
    : n{num_states},  alpha{alpha}, beta{beta}, kappa{kappa} {

    if (!n || (n < 4 || n > 50)) {
        Serial.println("SigmaPoints: num_states missing or out of bounds!");
        return;
    }
    if (!alpha || !beta || !kappa) {
        Serial.println("SigmaPoints: alpha, beta, or gamma missing!");
        return;
    }
}