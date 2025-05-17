#include <dps310.h>

DPS310::DPS310(uint8_t i2c_address) : address {i2c_address} {}

void DPS310::init() {
    i2c.init(address);
    
    uint8_t id = i2c.read_register(ID);
    Serial.print("DPS310 ID (16 expected): ");
    Serial.println(id);
    
    while (!(i2c.read_register(MEAS_CFG) & 0x80)) {
        delay(1000);
        Serial.println("waiting on calibration coefficients");
    }
    
    // Interrupt active low, generate interrupt when pressure ready, no bit shifts, disable fifo
    i2c.write_register(CFG_REG, 0b00110000);

    // 0 if using ASIC temperature sensor, 1 if using MEMS element temp sensor 
    uint8_t TMP_COEF_SRCE = i2c.read_register(0x28) >> 7;

    i2c.write_register(PRS_CFG, 0b01110000); // sets to 128hz, no oversampling

    // 128hz temp, set which temperature source is used
    uint8_t tmp_conf = 0b01110000;
    if (TMP_COEF_SRCE) tmp_conf |= 0x80;
    i2c.write_register(TMP_CFG, tmp_conf);

    // continuous pressure and temp measurements
    i2c.write_register(MEAS_CFG, 0b00000111);

    // flush fifo
    i2c.write_register(0x0C, 0x80);
    i2c.read_register(INT_STS);
    get_calibration_coefs();
}

size_t DPS310::fetch(SensorPacket* buffer, size_t buffer_index) {
    // checks if pressure and temp measurements are available
    uint8_t data_ready = i2c.read_register(MEAS_CFG);
    
    // resets the interrupt pins, must be read so that interrupt pin will return to high
    i2c.read_register(INT_STS);

    // returning early if both pressure and temp data isnt ready
    // bits 4 and 5 are pressure ready and temp ready, respectively
    if ((data_ready & 0x30) != 0x30) {
        //Serial.print("DPS: ");
        //Serial.println(buffer_index);
        return buffer_index;
    }

    SensorPacket packet;
    packet.timestamp = micros();
    // burst read measurements (pressure then temp)
    i2c.read_registers(0x00, bytes, 6);
    raw_pressure = twos_complement_24(bytes[0], bytes[1], bytes[2]);
    raw_temp = twos_complement_24(bytes[3], bytes[4], bytes[5]);
    packet.pressure = calculate_pressure();
    packet.temperature = calculate_temp();
    
    buffer[buffer_index] = packet;
    return ++buffer_index;
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
    i2c.read_registers(0x10, raw_coefs, NUM_ADDR_COEFS);
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
