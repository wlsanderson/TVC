#include <dps310.h>

#include <tvc_utils.h>


// Constructor for normal use
DPS310::DPS310(int CS_pin) {
    spi = new SPIUtils(spi_speed, read_byte, spi_mode, bit_order, CS_pin);
    owns_spi = true;
}

// Constructor for dependency injection
DPS310::DPS310(SPIUtils* spi_utils) {
    spi = spi_utils;
    owns_spi = false;
}

void DPS310::init() {
    uint8_t id = spi->read_register(ID);
    Serial.println(id);
    while (!(spi->read_register(MEAS_CFG) & 0x80)) {
        delay(1500);
        Serial.println("waiting on calibration coefficients");
    }
    const uint8_t TMP_COEF_SRCE = spi->read_register(0x28) >> 7;
    spi->write_register(PRS_CFG, 0B00100000); // sets to 8hz, no oversampling
    uint8_t tmp_conf = 0b00100000;
    if (TMP_COEF_SRCE) tmp_conf |= 0x80;
    spi->write_register(TMP_CFG, tmp_conf);
    spi->write_register(CFG_REG, 0b00000010); // no , no bit shifting, no interrupts
    spi->write_register(MEAS_CFG, 0b00000111);
    get_calibration_coefs();
}

void DPS310::fetch(std::queue<SensorPacket>& packet_queue) {
    uint8_t data_ready = spi->read_register(MEAS_CFG);
    // bit 4 is pressure ready
    // bit 5 is temp ready
    while (data_ready == (data_ready | 0x81)) { // checking if both are ready
        SensorPacket packet;
        packet.timestamp = micros();
        for (int i=0; i < 2; i++) {
            uint8_t bytes[3];
            spi->read_registers(0x00, bytes, 3);
            int32_t value = twos_complement_24(bytes[0], bytes[1], bytes[2]);
            // when FIFO is empty, registers will read 0x800000. With two's complement, this will be
            // negative 0x800000
            if (value == -0x800000) { // default value if the fifo is empty
                break;
            }

            if (bytes[2] & 0x01) { // LSB = 1 -> pressure measurement
                raw_pressure = value;
                packet.pressure = calculate_pressure();
            } else { // LSB = 0 -> temperature measurement
                raw_temp = value;
                packet.temperature = calculate_temp();
            }
            
        }
        packet_queue.push(packet);
        data_ready = spi->read_register(MEAS_CFG);
    }
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
    uint8_t raw_coefs[NUM_ADDR_COEFS];
    spi->read_registers(0x10, raw_coefs, NUM_ADDR_COEFS);
    c0 = twos_complement_12_hi(raw_coefs[0], raw_coefs[1]);
    c1 = twos_complement_12_lo(raw_coefs[1], raw_coefs[2]);
    c00 = twos_complement_20_hi(raw_coefs[3], raw_coefs[4], raw_coefs[5]);
    c10 = twos_complement_20_lo(raw_coefs[5], raw_coefs[6], raw_coefs[7]);
    c01 = twos_complement_16(raw_coefs[8], raw_coefs[9]);
    c11 = twos_complement_16(raw_coefs[10], raw_coefs[11]);
    c20 = twos_complement_16(raw_coefs[12], raw_coefs[13]);
    c21 = twos_complement_16(raw_coefs[14], raw_coefs[15]);
    c30 = twos_complement_16(raw_coefs[16], raw_coefs[17]);
}

DPS310::~DPS310() {
    if (owns_spi) {
        delete spi;
    }
}
