#include <lsm9ds1.h>

IMU::IMU(uint8_t lsm9ds1_imu_address, uint8_t lsm9ds1_mag_address)
    : imu_address {lsm9ds1_imu_address},
    mag_address {lsm9ds1_mag_address} {}

int IMU::begin() {
    i2c_imu.begin(imu_address);
    i2c_mag.begin(mag_address);
    delay(200);
    if (startup()) {
        return 1;
    }

    /* Enable block data update */
    i2c_imu.write_register(CTRL_REG8, 0x40 | i2c_imu.read_register(CTRL_REG8));
    i2c_mag.write_register(CTRL_REG5_M, 0x40);

    /*
    As described in section 3.1, starting the IMU with both the accelerometer and gyroscope
    active is done by writing to CTRL_REG1_G, which activates both at the same ODR. Must be done
    while in power down mode (CTRL_REG6_XL = PD). Gyroscope can not be the only one enabled. To
    enable accelerometer and not gyro, write to CTRL_REG6_XL first.
    */
    i2c_imu.write_register(CTRL_REG1_G, IMU_476Hz << 5);
    i2c_imu.write_register(CTRL_REG6_XL, IMU_476Hz << 5); // apparently you still need to do this idk.

    /*
    Section 3.1 also defines operating modes for magnetometer: power-down (default),
    continuous-conversion, and single-conversion. Continuous means the magnetometer is always
    collecting and replacing data, single-conversion is one-shot. You must request the data
    to be fetched
    */
    i2c_mag.write_register(CTRL_REG3_M, 0x00); // continuous conversion ([1:0] = 00)
    i2c_mag.write_register(CTRL_REG1_M, 0x80 | (MAG_560Hz << 1)); // force temp compensation
    i2c_mag.write_register(CTRL_REG4_M, (MAG_560Hz & 0x30) >> 2); // sets Z axis mode to the same
    
    /*
    Gyroscope can either be in power-down, low-power, or normal mode. To enable LP mode, LP_mode
    bit in CTRL_REG3_G must be set to 1. Low-power mode just uses lower power, it is only available
    for ODR of 14.9, 59.5, or 119Hz. 
    */
    i2c_imu.write_register(CTRL_REG3_G, 0x00); // forcing normal mode

    /* Set fullscale ranges and bandwidths for accelerometer, gyroscope, and magnetometer.*/
    i2c_imu.write_register(CTRL_REG1_G, (DPS_2000 << 3) | i2c_imu.read_register(CTRL_REG1_G));
    i2c_imu.write_register(CTRL_REG6_XL, (GS_16 << 3) | i2c_imu.read_register(CTRL_REG6_XL));
    i2c_mag.write_register(CTRL_REG2_M, GAUSS_4 << 5);

    /* Configure filtering chain. */
        // anti-aliasing filter for accelerometer
    i2c_imu.write_register(CTRL_REG6_XL, AUTO | i2c_imu.read_register(CTRL_REG6_XL));
        // lowpass filter for accelerometer
    i2c_imu.write_register(CTRL_REG7_XL, LP_DISABLE);
        // lowpass filter for gyroscope
    i2c_imu.write_register(CTRL_REG1_G, G_LP_ULTRALIGHT | i2c_imu.read_register(CTRL_REG1_G));
        // highpass filter for gyroscope
    i2c_imu.write_register(CTRL_REG2_G, 0x00); // disables highpass and LFP2 usage

    /*
    Section 3.5 desecribes FIFO operation. There are 32 slots for each of the gyro's outputs, and
    32 slots for each of the accelerometer's outputs. It has 5 different modes listed below:
        - Bypass mode: FIFO is not operational, can be used to reset FIFO when in FIFO mode
        - FIFO-mode: Data stored in FIFO until overwritten
        - Continuous mode: as new data arrives, older data is discarded in FIFO
        - Continuous-to-FIFO mode: interrupt source can switch between continuous and FIFO mode
        - Bypass-to-Continuous mode: interrupt source switches between bypass and continuous mode
    Each mode is selected by FMODE[2:0] bits in FIFO_CTRL.
    A FIFO threshold flag can be asserted in bit FTH at FIFO_SRC when the number of unread samples
    in the continuous FIFO is greater than or equal to FIFO_CTRL bits FTH[4:0].
    */
        // hardcodes threshold to 1
    i2c_imu.write_register(FIFO_CTRL, (CONTINUOUS_MODE << 5) | 0x01);
        // TODO: figure out why Bypass mode requires FIFO_EN=1, and how Bypass != FIFO off
    i2c_imu.write_register(CTRL_REG9, 0x02); // Hardcoding FIFO enable.

    /*
    Can route the FIFO threshold flag in FIFO_SRC to the INT1_A/G interrupt pin by writing in
    register INT1_CTRL INT1_FTH=1.
    */
    i2c_imu.write_register(INT1_CTRL, 0x08); 



    // discard samples so that the first few are clean
    if (discard()) {
        Serial.println("LSM9DS1: Sample discard failed!");
        return 1;
    }
    return 0;
}


size_t IMU::fetch_imu_mag(SensorPacket* buffer, unsigned int buffer_index) {
    // bits 0 and 1 are new accelerometer data and new gyro data, respectively
    uint8_t fifo = i2c_imu.read_register(FIFO_SRC);
    SensorPacket packet;
    bool has_data = false;
    if (fifo) {
        // clearing out the entire fifo, only logging most recent packet though
        size_t overrun = 0; // kill infinite loop if i2c not fast enough
        while (fifo) {
            hold_interrupt = true;
            i2c_imu.read_registers(OUT_X_G, (uint8_t*)imu_data, 12);
            hold_interrupt = false;
            fifo = i2c_imu.read_register(FIFO_SRC);
            overrun++;

            if (overrun > 64) {
                Serial.println("LSM9DS1: ERROR - Can't read samples fast enough!");
                break;
            }
        }
        packet.timestamp = micros();
        packet.gyro_x = imu_data[0];
        packet.gyro_y = imu_data[1];
        packet.gyro_z = imu_data[2];
        packet.acc_x = imu_data[3];
        packet.acc_y = imu_data[4];
        packet.acc_z = imu_data[5];
        
        has_data = true;
    }
    
    uint8_t mag_data_ready = i2c_mag.read_register(STATUS_REG_M);
    // bit 3 is 1 if X, Y, and Z magnetometer data is available
    if ((mag_data_ready & 0x08) == 0x08) {
        if (!packet.timestamp) {
            packet.timestamp = micros();
        }
        i2c_mag.read_registers(OUT_X_L_M, (uint8_t*)mag_data, 6);

        packet.mag_x = mag_data[0];
        packet.mag_y = mag_data[1];
        packet.mag_z = mag_data[2];
        has_data = true;
    }
    
    if (has_data) {
        
        buffer[buffer_index] = packet;
        return ++buffer_index;
    }

    // data was not fetched
    Serial.println("Didn't collect data");
    return buffer_index;
}

int IMU::startup() {
    // check for proper connections and i2c by reading ID
    if (i2c_imu.read_register(WHO_AM_I) != 0x68) {
        Serial.println("Could not detect LSM9DS1 IMU");
        return 1;
    }
    if (i2c_mag.read_register(WHO_AM_I_M) != 0x3D) {
        Serial.println("Could not detect LSM9DS1 Magnetometer");
        return 1;
    }

    // reboot + reset IMU to default registers
    i2c_imu.write_register(CTRL_REG8, 0x81 | i2c_imu.read_register(CTRL_REG8));
    bool imu_booted = false;
    for (size_t i = 0; i < 5; i++) {
        // check if default bits set
        if ((i2c_imu.read_register(WHO_AM_I) != 0x68)) {
            Serial.println("LSM9DS1 IMU: Booting...");
            delay(300);
        } else {
            imu_booted = true;
            break;
        }
    }
    // reboot + reset magnetometer to default registers
    i2c_mag.write_register(CTRL_REG2_M, 0x0C | i2c_mag.read_register(CTRL_REG2_M));
    bool mag_booted = false;
    for (size_t i = 0; i < 5; i++) {
        // check if default bits set
        if ((i2c_mag.read_register(WHO_AM_I_M) != 0x3D)) {
            Serial.println("LSM9DS1 Mag: Booting...");
            delay(300);
        } else {
            mag_booted = true;
            break;
        }
    }
    if (!(imu_booted && mag_booted)) {
        return 1;
    }
    return 0;
}

int IMU::discard() {
    int imu_hz = (i2c_imu.read_register(CTRL_REG1_G) & 0xE0) >> 5;
    size_t num_cycles = 0;
    int delay_time = 0;
    switch (imu_hz) {
        case IMU_PD:
            return 1; // IMU should not be powered down at this point
        case IMU_14Hz9:
            num_cycles = 1;
            delay_time = 100;
            break;
        case IMU_59Hz5:
            num_cycles = 1;
            delay_time = 50;
            break;
        case IMU_119Hz:
            num_cycles = 3;
            delay_time = 20;
            break;
        case IMU_238Hz:
            num_cycles = 5;
            delay_time = 8;
            break;
        case IMU_476Hz:
            num_cycles = 8;
            delay_time = 4;
            break;
        case IMU_952Hz:
            num_cycles = 15;
            delay_time = 3;
            break;
    }
    if (num_cycles == 0) {
        return 1;
    }
    
    // loop read
    for (uint8_t i = 0; i < num_cycles; i++) {
        int16_t bytes_imu[6];
        i2c_imu.read_registers(OUT_X_G, (uint8_t*)bytes_imu, sizeof(bytes_imu));
        int16_t bytes_mag[3];
        i2c_mag.read_registers(OUT_X_L_M, (uint8_t*)bytes_mag, sizeof(bytes_mag));
        delay(delay_time);
    }
    return 0;
}
