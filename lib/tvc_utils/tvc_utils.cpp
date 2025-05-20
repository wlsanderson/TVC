#include <tvc_utils.h>
#include <Arduino.h>

int16_t twos_complement_12_hi(uint8_t msb_8, uint8_t lsb_4) {
    if (lsb_4 & 0xF0) lsb_4 >>= 4;
    int16_t val = (int16_t)((msb_8 << 4) | lsb_4);
    if (val & 0x0800) val |= 0xF000;
    return val;
}
int16_t twos_complement_12_lo(uint8_t msb_4, uint8_t lsb_8) {
    int16_t val = (int16_t)(((msb_4 & 0x0F) << 8) | lsb_8);
    if (val & 0x0800) val |= 0xF000;
    return val;
}
int16_t twos_complement_16(uint8_t msb, uint8_t lsb) {
    return (int16_t)((msb << 8) | lsb);
}
int32_t twos_complement_20_hi(uint8_t msb_8, uint8_t lsb_8, uint8_t xlsb_4) {
    if (xlsb_4 & 0xF0) xlsb_4 >>= 4;
    int32_t val = (int32_t)((msb_8 << 12) | (lsb_8 << 4) | xlsb_4);
    if (val & 0x00080000) val |= 0xFFF00000;
    return val;
}
int32_t twos_complement_20_lo(uint8_t msb_4, uint8_t lsb_8, uint8_t xlsb_8) {
    int32_t val = (int32_t)(((msb_4 & 0x0F) << 16) | (lsb_8 << 8) | xlsb_8);
    if (val & 0x00080000) val |= 0xFFF00000;
    return val;
}
int32_t twos_complement_24(uint8_t msb, uint8_t lsb, uint8_t xlsb) {
    int32_t val = (int32_t)((msb << 16) | (lsb << 8) | xlsb);
    if (val & 0x00800000) val |= 0xFF000000;
    return val;
}

Vector4d acc_mag_to_quat(Vector3d acc, Vector3d mag) {
    Vector3d north = (acc.cross(mag)).cross(acc);
    Vector3d east = acc.cross(mag);
    Vector3d down = acc;
    Matrix3d rotmat;
    rotmat.col(0) = north.normalized();
    rotmat.col(1) = east.normalized();
    rotmat.col(2) = down.normalized();
    rotmat.transposeInPlace();
    Quaterniond quat(rotmat);
    Vector4d ret(quat.w(), quat.x(), quat.y(), quat.z());
    return ret;
}
