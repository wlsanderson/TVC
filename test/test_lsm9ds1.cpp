// #include <lsm9ds1.h>

// #include <gtest/gtest.h>
// #include <ArduinoFake.h>
// #include <gmock/gmock.h>


// using namespace fakeit;

// using ::testing::Return;
// using ::testing::InSequence;
// using ::testing::_;

// class MockI2CUtils : public I2CUtils {
//     public: 
//         MockI2CUtils() : I2CUtils(0, 0) {}
//         MOCK_METHOD(uint8_t, read_register, (uint8_t reg_addr), (override));
//         MOCK_METHOD(void, read_registers, (uint8_t reg_addr, uint8_t* buffer, int num_bytes), (override));
//         MOCK_METHOD(void, write_register, (uint8_t reg_addr, uint8_t value), (override));   
// };

// TEST(LSM9DS1Test, fetch_imu_creates_correct_packet) {
//     MockI2CUtils mock_i2c_imu;
//     MockI2CUtils mock_i2c_mag;
//     IMU mock_imu(&mock_i2c_imu, &mock_i2c_mag);

//     When(Method(ArduinoFake(), micros)).AlwaysReturn(100);
//     When(OverloadedMethod(ArduinoFake(Serial), println, size_t(unsigned char,int))).AlwaysReturn();

//     {
//         InSequence s;
        
//         // calls and sets number of unread samples in FIFO to 2 (one acc, one gyro)
//         EXPECT_CALL(mock_i2c_imu, read_register(FIFO_SRC))
//             .WillOnce(Return(0x02));
        
//         // reads 12 output registries (6 gyro, 6 accel)
//         EXPECT_CALL(mock_i2c_imu, read_registers(OUT_X_G, testing::_, 12))
//             .WillOnce([](uint8_t, uint8_t* buffer, uint8_t num_bytes) {
//                 buffer[0] = 0x9A; // gyro_x = 28.7 deg/s
//                 buffer[1] = 0x01;
//                 buffer[2] = 0x49; // gyro_y = 5.11 deg/s
//                 buffer[3] = 0x00;
//                 buffer[4] = 0x00; // gyro_z = -89.6 deg/s
//                 buffer[5] = 0xFB;
//                 buffer[6] = 0xFF; // acc_x = 0.561444 g
//                 buffer[7] = 0x02;
//                 buffer[8] = 0xAA; // acc_y = 3.310104 g
//                 buffer[9] = 0x11;
//                 buffer[10] = 0x98; //acc_z = 1.048224 g
//                 buffer[11] = 0x05;
//             });
        
//         // re-read unread samples 
//         EXPECT_CALL(mock_i2c_imu, read_register(FIFO_SRC))
//             .WillOnce(Return(0xC0)); // also confirming that only bits [5:0] kill the loop
//     }

//     std::queue<SensorPacket> packet_queue;
//     mock_imu.fetch_imu(packet_queue);

//     EXPECT_EQ(packet_queue.size(), 1);
//     SensorPacket packet = packet_queue.front();
//     EXPECT_NEAR(packet.gyro_x, 28.7, 0.01);
//     EXPECT_NEAR(packet.gyro_y, 5.11, 0.01);
//     EXPECT_NEAR(packet.gyro_z, -89.6, 0.01);
//     EXPECT_NEAR(packet.acc_x, 0.561444, 0.00001);
//     EXPECT_NEAR(packet.acc_y, 3.310104, 0.00001);
//     EXPECT_NEAR(packet.acc_z, 1.048224, 0.00001);
// }

// TEST(LSM9DS1Test, fetch_mag_creates_correct_packet) {
//     MockI2CUtils mock_i2c_imu;
//     MockI2CUtils mock_i2c_mag;
//     IMU mock_imu(&mock_i2c_imu, &mock_i2c_mag);

//     When(Method(ArduinoFake(), micros)).AlwaysReturn(100);
//     When(OverloadedMethod(ArduinoFake(Serial), println, size_t(unsigned char,int))).AlwaysReturn();

//     {
//         InSequence s;
        
//         // reads if magnetometer data is ready (bit 4 only)
//         EXPECT_CALL(mock_i2c_mag, read_register(STATUS_REG_M))
//             .WillOnce(Return(0x2A));

//         // reads 6 output registries for magnetometer
//         EXPECT_CALL(mock_i2c_mag, read_registers(OUT_X_L_M, testing::_, 6))
//             .WillOnce([](uint8_t, uint8_t* buffer, uint8_t num_bytes) {
//                 buffer[0] = 0x9A; // mag_x = 57.4 gauss
//                 buffer[1] = 0x01;
//                 buffer[2] = 0x49; // mag_y = 10.22 gauss
//                 buffer[3] = 0x00;
//                 buffer[4] = 0x00; // mag_z = -179.2 gauss
//                 buffer[5] = 0xFB;
//             });
        
//         // re-read unread samples 
//         EXPECT_CALL(mock_i2c_mag, read_register(STATUS_REG_M))
//             .WillOnce(Return(0x37)); // bit 4 = 0, data not ready
//     }

//     std::queue<SensorPacket> packet_queue;
//     mock_imu.fetch_mag(packet_queue);

//     EXPECT_EQ(packet_queue.size(), 1);
//     SensorPacket packet = packet_queue.front();
//     EXPECT_NEAR(packet.mag_x, 57.4, 0.00001);
//     EXPECT_NEAR(packet.mag_y, 10.22, 0.00001);
//     EXPECT_NEAR(packet.mag_z, -179.2, 0.00001);
// }