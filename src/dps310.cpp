#include "dps310.h"
#include "constants.h"
#include "spi_utils.h"
#include "sensor_packet.h"


void DPS310::init() {
    Serial.println(spi_read_register(ID, CS_PIN_DPS310));
    while (!(spi_read_register(MEAS_CFG, CS_PIN_DPS310) & 0x80)) {
        delay(1500);
        Serial.println("waiting on calibration coefficients");
    }
    const uint8_t TMP_COEF_SRCE = spi_read_register(0x28, CS_PIN_DPS310) >> 7;
    spi_write_register(PRS_CFG, 0B00100000, CS_PIN_DPS310); // sets to 8hz, no oversampling
    uint8_t tmp_conf = 0b00100000;
    if (TMP_COEF_SRCE) tmp_conf |= 0x80;
    spi_write_register(TMP_CFG, tmp_conf, CS_PIN_DPS310);
    spi_write_register(CFG_REG, 0b00000010, CS_PIN_DPS310); // no FIFO, no bit shifting, no interrupts
    spi_write_register(MEAS_CFG, 0b00000111, CS_PIN_DPS310);
    get_calibration_coefs();
}

void DPS310::fetch(std::queue<SensorPacket>& packet_queue) {
    uint8_t fifo_empty = spi_read_register(FIFO_STS, CS_PIN_DPS310) & 0x01; // LSB determines if empty or not
    int max_reads = dps310_sensor_buffer / 2; // FIFO buffer size is 32, dont loop more than 32 times
    while (!fifo_empty && max_reads-- > 0) {
        SensorPacket packet;
        packet.timestamp = micros();
        for (int i=0; i < 2; i++) {
            
            int32_t value = read_next();
            if (value == 0x800000) { // default value if the fifo is empty
                break;
            }
            if (value & 0x800000) value |= 0xFF000000; // two's complement

            if (value & 0x01) { // LSB = 1 -> pressure measurement
                raw_pressure = value;
                packet.pressure = calculate_pressure();
            } else { // LSB = 0 -> temperature measurement
                raw_temp = value;
                packet.temperature = calculate_temp();
            }
            fifo_empty = spi_read_register(FIFO_STS, CS_PIN_DPS310) & 0x01;
        }
        packet_queue.push(packet);
    }
}

int32_t DPS310::read_next() {
    uint8_t pt_bytes[3];
    spi_read_registers(0x00, pt_bytes, 3, CS_PIN_DPS310);
    
    int32_t value = (int32_t)((pt_bytes[0] << 16) | (pt_bytes[1] << 8) | pt_bytes[2]);
    if (value == 0x800000) { // default value if fifo is empty
        delay(10); // TODO: this is really bad and unreliable
        uint8_t pt_bytes[3];
        spi_read_registers(0x00, pt_bytes, 3, CS_PIN_DPS310);
        value = (int32_t)((pt_bytes[0] << 16) | (pt_bytes[1] << 8) | pt_bytes[2]);
    }
    return value;
}

float DPS310::calculate_pressure() {
    float T_raw_sc = (float)raw_temp / kT;
    float P_raw_sc = (float)raw_pressure / kP;
    return c00 + P_raw_sc * (c10 + P_raw_sc * (c20 + P_raw_sc * c30)) + T_raw_sc * c01 + T_raw_sc * P_raw_sc * (c11 + P_raw_sc * c21);
}

float DPS310::calculate_temp() {
    float T_raw_sc = (float)raw_temp / kT;
    return c0 * 0.5 + c1 * T_raw_sc;
}

void DPS310::get_calibration_coefs() {
    uint8_t raw_coef_buffer[NUM_ADDR_COEFS];
    spi_read_registers(0x10, raw_coef_buffer, NUM_ADDR_COEFS, CS_PIN_DPS310);

    c0 = (int16_t)((raw_coef_buffer[0] << 4) | ((raw_coef_buffer[1] >> 4) & 0x0F));
    if (c0 & 0x0800) c0 |= 0xF000;
    c1 = (int16_t)(((raw_coef_buffer[1] & 0x0F) << 8) | raw_coef_buffer[2]);
    if (c1 & 0x0800) c1 |= 0xF000;
    c00 = (int32_t)(raw_coef_buffer[3] << 12) | (raw_coef_buffer[4] << 4) | ((raw_coef_buffer[5] >> 4) & 0x0F);
    if (c00 & 0x080000) c00 |= 0xFFF00000;
    c10 = (int32_t)((raw_coef_buffer[5] & 0x0F) << 16) | (raw_coef_buffer[6] << 8) | raw_coef_buffer[7];
    if (c10 & 0x080000) c10 |= 0xFFF00000;
    c01 = (int16_t)((raw_coef_buffer[8] << 8) | raw_coef_buffer[9]);
    c11 = (int16_t)((raw_coef_buffer[10] << 8) | raw_coef_buffer[11]);
    c20 = (int16_t)((raw_coef_buffer[12] << 8) | raw_coef_buffer[13]);
    c21 = (int16_t)((raw_coef_buffer[14] << 8) | raw_coef_buffer[15]);
    c30 = (int16_t)((raw_coef_buffer[16] << 8) | raw_coef_buffer[17]);
}
