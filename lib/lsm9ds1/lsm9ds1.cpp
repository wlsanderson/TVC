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


