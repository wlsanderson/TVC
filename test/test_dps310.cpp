#include <ArduinoFake.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <dps310.h>

using namespace fakeit;

using ::testing::Return;
using ::testing::InSequence;
using ::testing::_;

class MockSPIUtils : public SPIUtils {
    public: 
        MockSPIUtils() : SPIUtils(0, 0, 0, 0, 0) {}
        MOCK_METHOD(uint8_t, read_register, (uint8_t addr), (override));
        MOCK_METHOD(void, read_registers, (uint8_t start_addr, uint8_t* buffer, int num_bytes), (override));
        MOCK_METHOD(void, write_register, (uint8_t addr, uint8_t value), (override));
        
};

TEST(DPS310Test, fetch_creates_correct_packet) {
    
    MockSPIUtils mock_spi;
    DPS310 mock_dps310(&mock_spi);

    When(Method(ArduinoFake(), micros)).AlwaysReturn(100);
    //When(OverloadedMethod(ArduinoFake(Serial), println, size_t(const char *))).AlwaysReturn();
    When(OverloadedMethod(ArduinoFake(Serial), println, size_t(unsigned char,int))).AlwaysReturn();

    {
        InSequence s;
        EXPECT_CALL(mock_spi, read_register(ID))
            .WillOnce(Return(0xFF));
        EXPECT_CALL(mock_spi, read_register(MEAS_CFG))
            .WillOnce(Return(0x80));
        EXPECT_CALL(mock_spi, read_register(0x28))
            .WillOnce(Return(0xFF));

        EXPECT_CALL(mock_spi, write_register(PRS_CFG, testing::_));
        EXPECT_CALL(mock_spi, write_register(TMP_CFG, testing::_));
        EXPECT_CALL(mock_spi, write_register(CFG_REG, testing::_));
        EXPECT_CALL(mock_spi, write_register(MEAS_CFG, testing::_));

        // Set calibration coefficients
        EXPECT_CALL(mock_spi, read_registers(0x10, testing::_, testing::_))
            .WillOnce([](uint8_t, uint8_t* buffer, uint8_t num_bytes) {
                buffer[0] = 0xFF;
                buffer[1] = 0xFF;
                buffer[2] = 0xFF;
                buffer[3] = 0xFF;
                buffer[4] = 0xFF;
                buffer[5] = 0xFF;
                buffer[6] = 0xFF;
                buffer[7] = 0xFF;
                buffer[8] = 0xFF;
                buffer[9] = 0xFF;
                buffer[10] = 0xFF;
                buffer[11] = 0xFF;
                buffer[12] = 0xFF;
                buffer[13] = 0xFF;
                buffer[14] = 0xFF;
                buffer[15] = 0xFF;
                buffer[16] = 0xFF;
                buffer[17] = 0xFF;
            });

        // Simulate pressure and temp ready
        EXPECT_CALL(mock_spi, read_register(MEAS_CFG))
            .WillOnce(Return(0x81));
        
        EXPECT_CALL(mock_spi, read_registers(testing::_, testing::_, testing::_))
            .WillOnce([](uint8_t, uint8_t* buffer, uint8_t num_bytes) {
                buffer[0] = 0x01; buffer[1] = 0x02; buffer[2] = 0x03;
            })
            .WillOnce([](uint8_t, uint8_t* buffer, uint8_t num_bytes) {
                buffer[0] = 0x04; buffer[1] = 0x05; buffer[2] = 0x06;
            });
        EXPECT_CALL(mock_spi, read_register(MEAS_CFG))
            .WillOnce(Return(0x00));
    }

    std::queue<SensorPacket> packet_queue;
    mock_dps310.init();
    mock_dps310.fetch(packet_queue);

    EXPECT_EQ(packet_queue.size(), 1);
    EXPECT_NEAR(packet_queue.front().pressure, 100000, 0.01);
}