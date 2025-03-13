#include "constants.h"
#include "spi_utils.h"
#include "dps310.h"


// DPS310 calibration coefficients
int16_t c0 = 0;
int16_t c1 = 0;
int32_t c00 = 0;
int32_t c10 = 0;
int16_t c01 = 0;
int16_t c11 = 0;
int16_t c20 = 0;
int16_t c21 = 0;
int16_t c30 = 0;

int32_t raw_temp = 0;
int32_t raw_pressure = 0;
unsigned long int p_count = 0;
unsigned long int t_count = 0;
unsigned long int last_time = 0;



void get_calibration_coefs() {
    uint8_t raw_coef_buffer[NUM_ADDR_COEFS];
    readRegisters(0x10, raw_coef_buffer, NUM_ADDR_COEFS);

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
    // Serial.print("c0 = "); Serial.println(c0);
    // Serial.print("c1 = "); Serial.println(c1);
    // Serial.print("c00 = "); Serial.println(c00);
    // Serial.print("c10 = "); Serial.println(c10);
    // Serial.print("c01 = "); Serial.println(c01);
    // Serial.print("c11 = "); Serial.println(c11);
    // Serial.print("c20 = "); Serial.println(c20);
    // Serial.print("c21 = "); Serial.println(c21);
    // Serial.print("c30 = "); Serial.println(c30);
}

float calculate_pressure(int32_t pressure_raw, int32_t temp_raw) {
    float T_raw_sc = (float)temp_raw / kT;
    float P_raw_sc = (float)pressure_raw / kP;
    return c00 + P_raw_sc * (c10 + P_raw_sc * (c20 + P_raw_sc * c30)) + T_raw_sc * c01 + T_raw_sc * P_raw_sc * (c11 + P_raw_sc * c21);
}

float calculate_temp(int32_t temp_raw) {
    float T_raw_sc = (float)temp_raw / kT;
    return c0 * 0.5 + c1 * T_raw_sc;
}

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        delay(50);
    }
    SPI.begin();
    pinMode(CS_PIN, OUTPUT);
    DPS310 dps310;

    if (!(readRegister(MEAS_CFG) & 0x80)) {
        delay(5);
        Serial.println("waiting on calibration coefficients");
    }
    const uint8_t TMP_COEF_SRCE = readRegister(0x28) >> 7;
    get_calibration_coefs();
    writeRegister(PRS_CFG, 0B00100000); // sets to 8hz, no oversampling
    uint8_t tmp_conf = 0b00100000;
    if (TMP_COEF_SRCE) tmp_conf |= 0x80;
    writeRegister(TMP_CFG, tmp_conf);
    writeRegister(CFG_REG, 0b00000010); // no FIFO, no bit shifting, no interrupts
    writeRegister(MEAS_CFG, 0b00000111);
    
}

void loop() {
    uint8_t fifo_empty = readRegister(FIFO_STS) & 0x01;
    int max_reads = 32; // FIFO buffer size is 32, dont loop more than 32 times
    while (!fifo_empty && max_reads-- > 0) {
        uint8_t pt_bytes[3];
        readRegisters(0x00, pt_bytes, 3);
        if (pt_bytes[2] & 0x01) {
            raw_pressure = (int32_t)((pt_bytes[0] << 16) | (pt_bytes[1] << 8) | pt_bytes[2]);
            if (raw_pressure & 0x800000) raw_pressure |= 0xFF000000;
            p_count++;
        } else {
            raw_temp = (int32_t)((pt_bytes[0] << 16) | (pt_bytes[1] << 8) | pt_bytes[2]);
            if (raw_temp & 0x800000) raw_temp |= 0xFF000000;
            t_count++;
        }

        fifo_empty = readRegister(FIFO_STS) & 0x01;
    }
    uint64_t t = millis();
    if ((t - last_time) > 1000) {
        Serial.print("P Freq: "); Serial.println(p_count / 1.0);
        Serial.print("T Freq: "); Serial.println(t_count / 1.0);
        p_count = 0;
        t_count = 0;
        last_time = t;
    }
    
    
}
