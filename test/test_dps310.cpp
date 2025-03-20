#include <dps310.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ArduinoFake.h>


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


class DPS310ParamTests : public ::testing::TestWithParam<int> {};


TEST_P(DPS310ParamTests, testFetch) {
    MockSPIUtils mock_spi;
    DPS310 mock_dps310(&mock_spi);

    When(Method(ArduinoFake(), micros)).AlwaysReturn(100);
    When(OverloadedMethod(ArduinoFake(Serial), println, size_t(unsigned char,int))).AlwaysReturn();

    int expected_packets = GetParam();
    std::cout << "Test for number of packets: " << expected_packets << std::endl;

    {
        InSequence s;
        if (expected_packets) {
            EXPECT_CALL(mock_spi, read_register(MEAS_CFG))
                .WillOnce(Return(0x18));
        } else {
            EXPECT_CALL(mock_spi, read_register(MEAS_CFG))
                .WillOnce(Return(0x00));
        }
        for (int i = 0; i < expected_packets; i++) {
            EXPECT_CALL(mock_spi, read_registers(testing::_, testing::_, testing::_))
                .WillRepeatedly([](uint8_t, uint8_t* buffer, uint8_t num_bytes) {
                    buffer[0] = 0x01; buffer[1] = 0x02; buffer[2] = 0x03;
                });
            if (expected_packets - i > 1) {
                EXPECT_CALL(mock_spi, read_register(MEAS_CFG))
                    .WillOnce(Return(0x18));
            } else {
                // last packet, will not read another after this
                EXPECT_CALL(mock_spi, read_register(MEAS_CFG))
                    .WillOnce(Return(0x00));
            }
        }
    }
    

    std::queue<SensorPacket> packet_queue;
    mock_dps310.fetch(packet_queue);

    EXPECT_EQ(packet_queue.size(), expected_packets);

}

INSTANTIATE_TEST_SUITE_P(
    DPS310Tests,
    DPS310ParamTests,
    testing::Values(
        0,
        1,
        2
    ),
    testing::PrintToStringParamName()
);


class DPS310ParamValuesTests : public ::testing::TestWithParam<std::tuple<byte, byte, byte, byte, byte, byte, float, float>> {};

TEST_P(DPS310ParamValuesTests, testFetchValues) {
    MockSPIUtils mock_spi;
    DPS310 mock_dps310(&mock_spi);

    When(Method(ArduinoFake(), micros)).AlwaysReturn(100);
    When(OverloadedMethod(ArduinoFake(Serial), println, size_t(unsigned char,int))).AlwaysReturn();

    byte b1 = std::get<0>(GetParam());
    byte b2 = std::get<1>(GetParam());
    byte b3 = std::get<2>(GetParam());
    byte b4 = std::get<3>(GetParam());
    byte b5 = std::get<4>(GetParam());
    byte b6 = std::get<5>(GetParam());

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
        
        EXPECT_CALL(mock_spi, read_register(MEAS_CFG))
            .WillOnce(Return(0x18));
        EXPECT_CALL(mock_spi, read_registers(testing::_, testing::_, testing::_))
            .WillOnce([b1, b2, b3](uint8_t, uint8_t* buffer, uint8_t num_bytes) {
                buffer[0] = b1; buffer[1] = b2; buffer[2] = b3;
            })
            .WillOnce([b4, b5, b6](uint8_t, uint8_t* buffer, uint8_t num_bytes) {
                buffer[0] = b4; buffer[1] = b5; buffer[2] = b6;
            });

            // last packet, will not read another after this
            EXPECT_CALL(mock_spi, read_register(MEAS_CFG))
                .WillOnce(Return(0x00));
    }
    

    std::queue<SensorPacket> packet_queue;
    mock_dps310.init();
    mock_dps310.fetch(packet_queue);

    SensorPacket packet = packet_queue.front();
    EXPECT_NEAR(packet.pressure, std::get<6>(GetParam()), 0.0001);
    EXPECT_NEAR(packet.temperature, std::get<7>(GetParam()), 0.0001);

}

INSTANTIATE_TEST_SUITE_P(
    DPS310Tests,
    DPS310ParamValuesTests,
    testing::Values(
        std::make_tuple(0x01, 0x02, 0x03, 0x04, 0x05, 0x0F, -1.881807, 0.0), // 2 pressure values
        std::make_tuple(0xA8, 0x5B, 0xBC, 0x09, 0x12, 0x26, 0.0, -1.63386), // 2 temp values
        std::make_tuple(0x5B, 0x0F, 0xF1, 0x11, 0x9B, 0x76, -1616.79235, -2.700908), // 1 of each
        std::make_tuple(0x5B, 0x0F, 0xF1, 0x80, 0x00, 0x00, -1616.79235, 0.0) // breaks
    )
);
