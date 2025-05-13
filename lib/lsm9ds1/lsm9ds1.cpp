#include <lsm9ds1.h>
#include <etl/queue.h>


void IMU::init() {
    //i2c_write_register(imu_addr, 0x22, 0b00000101);
    i2c_imu.init(imu_addr, lsm9ds1_i2c_speed);
    i2c_mag.init(mag_addr, lsm9ds1_i2c_speed);

    Serial.print("LSM9DS1 IMU (0x68 expected):  ");
    Serial.println(i2c_imu.read_register(WHO_AM_I), HEX);

    Serial.print("LSM9DS1 MAG (0x3D expected): ");
    Serial.println(i2c_mag.read_register(WHO_AM_I_M), HEX);

    /*
    ACC/GYRO interrupt config
    
    INT1_IG_G: enable gyroscope interrupt generator on INT 1_A/G pin: 0
    INT_IG_XL: enable accelerometer interrupt generator on INT 1_A/G pin: 0
    INT_FSS5: enable fifo unread sample interrupt on INT 1_A/G: 0
    INT_OVR: enable overrun interrupt on INT 1_A/G: 0
    INT_FTH: enable fifo threshold interrupt on INT 1_A/G: 0
    INT_Boot: enable boot status on INT 1_A/G pin: 0
    INT_DRDY_G: enable gyroscope data ready on INT 1_A/G: 1
    INT_DRDY_XL: enable accelerometer data ready on INT 1_A/G: 1
    */
    i2c_imu.write_register(INT1_CTRL, 0b00000001);


    /*
    ACC/GYRO:

    0
    SLEEP_G: sleep mode enabled: 0
    0
    FIFO_TEMP_EN: temperature data stored in FIFO: 0
    DRDY_mask_bit: data available enable bit: 1
    I2C_DISABLE: disables I2C: 0 
    FIFO_EN: enables FIFO: 0
    STOP_ON_FTH: Enables FIFO threshold usage: 0
    */
    i2c_imu.write_register(CTRL_REG9, 0b00001000);

    /*
    MAG

    I2C_DISABLE: disables I2C: 0
    0
    LP: enables low-power mode: 0
    0
    0
    SIM: enables SPI: 0
    MD1/MD0: Operating Mode: 00 (continuous-conversion)
    */
    i2c_mag.write_register(CTRL_REG3_M, 0b00000000);


    i2c_imu.write_register(CTRL_REG1_G, 0b10111011); // sets gyro ODR/BW
    i2c_imu.write_register(CTRL_REG2_G, 0b00000010); // enables LPF2 usage


    i2c_imu.write_register(CTRL_REG6_XL, 0b10101000); // sets accel ODR, 16g cap, auto sets BW
    i2c_imu.write_register(CTRL_REG8, 0b00000100); // interrupt pulls pin high, push-pull mode

    i2c_mag.write_register(CTRL_REG1_M, 0b10011100); // temperature compensation, 80hz
    i2c_mag.write_register(CTRL_REG2_M, 0b00000000); // 4 gauss cap
    i2c_mag.write_register(CTRL_REG4_M, 0b0000000); // Low-power mode

    // // do a read to start the interrupt
    uint8_t bytes_mag[6];
    i2c_mag.read_registers(OUT_X_L_M, bytes_mag, 6);
}

int IMU::fetch_imu_mag(SensorPacket* buffer, unsigned int buffer_index) {
    // bits 0 and 1 are new accelerometer data and new gyro data, respectively
    uint8_t imu_data_ready = i2c_imu.read_register(STATUS_REG);

    SensorPacket packet;
    bool has_data = false;
    if ((imu_data_ready & 0x03) == 0x03) {
        // fetching both accel and gyro
        packet.timestamp = micros();
        // read gyro and accel
        // uint8_t bytes[12];
        // i2c_imu.read_registers(OUT_X_G, bytes, 12);
        // int16_t gx = twos_complement_16(bytes[1], bytes[0]);
        // int16_t gy = twos_complement_16(bytes[3], bytes[2]);
        // int16_t gz = twos_complement_16(bytes[5], bytes[4]);
        // int16_t ax = twos_complement_16(bytes[7], bytes[6]);
        // int16_t ay = twos_complement_16(bytes[9], bytes[8]);
        // int16_t az = twos_complement_16(bytes[11], bytes[10]);
        int16_t data[3];
        i2c_imu.read_registers(0x28, (uint8_t*)data, sizeof(data));

        int16_t gyro_data[3];
        i2c_imu.read_registers(OUT_X_G, (uint8_t*)gyro_data, sizeof(gyro_data));


        packet.gyro_x = (float)(gyro_data[0]) * gyro_scale_factor;
        packet.gyro_y = (float)(gyro_data[1]) * gyro_scale_factor;
        packet.gyro_z = (float)(gyro_data[2]) * gyro_scale_factor;
        packet.acc_x = (float)(data[0]) * acc_scale_factor;
        packet.acc_y = (float)(data[1]) * acc_scale_factor;
        packet.acc_z = (float)(data[2]) * acc_scale_factor;
        has_data = true;
        // if (abs(packet.acc_x) > 0.000000015) {
        //     Serial.print(bytes[7],BIN);
        //     Serial.print(" ");
        //     Serial.println(bytes[6], BIN);
        // }
        if(abs(packet.acc_x) > 0.15) {
            //Serial.print(bytes[7],BIN);
            Serial.print(" x ");
            Serial.println(data[0], BIN);
        }
        if(abs(packet.acc_y) > 0.15) {
            //Serial.print(bytes[9],BIN);
            Serial.print(" y ");
            Serial.println(data[1], BIN);
        }
    }
    uint8_t mag_data_ready = i2c_mag.read_register(STATUS_REG_M);
    // bit 3 is 1 if X, Y, and Z magnetometer data is available
    if ((mag_data_ready & 0x08) == 0x08) {
        if (!packet.timestamp) {
            packet.timestamp = micros();
        }
        uint8_t bytes[6];
        i2c_mag.read_registers(OUT_X_L_M, bytes, 6);
        int16_t mag_x = twos_complement_16(bytes[1], bytes[0]);
        int16_t mag_y = twos_complement_16(bytes[3], bytes[2]);
        int16_t mag_z = twos_complement_16(bytes[5], bytes[4]);

        packet.mag_x = (float)(mag_x) * mag_scale_factor;
        packet.mag_y = (float)(mag_y) * mag_scale_factor;
        packet.mag_z = (float)(mag_z) * mag_scale_factor;
        has_data = true;
    }
    if (has_data) {
        buffer[buffer_index] = packet;
        return ++buffer_index;
    }

    // data was not fetched
    //Serial.print("LSM: ");
    //Serial.println(buffer_index);
    return buffer_index;
}

