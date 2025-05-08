// #include <tvc_utils.h>

// #include <gtest/gtest.h>
// #include <ArduinoFake.h>



// TEST(TwosComplement12Hi, PositiveLSBxF0) {
//     EXPECT_EQ(twos_complement_12_hi(0b01000111, 0b01000000), 1140);
// }
// TEST(TwosComplement12Hi, PositiveLSBx0F) {
//     EXPECT_EQ(twos_complement_12_hi(0b00000100, 0b00000111), 71);
// }
// TEST(TwosComplement12Hi, NegativeLSBxF0) {
//     EXPECT_EQ(twos_complement_12_hi(0b11010100, 0b10100000), (0b110101001010 - 4096));
// }
// TEST(TwosComplement12Hi, NegativeLSBx0F) {
//     EXPECT_EQ(twos_complement_12_hi(0b10110100, 0b00001111), (0b101101001111 - 4096));
// }
// TEST(TwosComplement12Hi, ZeroLSB) {
//     EXPECT_EQ(twos_complement_12_hi(0b10110100, 0x00), (0b101101000000 - 4096));
// }
// TEST(TwosComplement12Hi, Zero) {
//     EXPECT_EQ(twos_complement_12_hi(0x00, 0x00), 0);
// }

// TEST(TwosComplement12Lo, PositiveMSBxF0) {
//     EXPECT_EQ(twos_complement_12_lo(0b01000000, 0b01000111), 1095);
// }
// TEST(TwosComplement12Lo, PositiveMSBx0F) {
//     EXPECT_EQ(twos_complement_12_lo(0b00000111, 0b01100100), 1892);
// }
// TEST(TwosComplement12Lo, NegativeMSBxF0) {
//     EXPECT_EQ(twos_complement_12_lo(0b10100000, 0b11010100), (0b101011010100 - 4096));
// }
// TEST(TwosComplement12Lo, NegativeMSBx0F) {
//     EXPECT_EQ(twos_complement_12_lo(0b00001111, 0b10110100), (0b111110110100 - 4096));
// }
// TEST(TwosComplement12Lo, ZeroMSB) {
//     EXPECT_EQ(twos_complement_12_lo(0x00, 0x65), 101);
// }
// TEST(TwosComplement12Lo, Zero) {
//     EXPECT_EQ(twos_complement_12_lo(0x00, 0x00), 0);
// }

// TEST(TwosComplement16, Positive) {
//     EXPECT_EQ(twos_complement_16(0b01110101, 0b11010010), 30162);
// }
// TEST(TwosComplement20Hi, PositiveLSBxF0) {
//     EXPECT_EQ(twos_complement_20_hi(0x4F, 0xAC, 0xA0), 326346);
// }
// TEST(TwosComplement20Hi, PositiveLSBx0F) {
//     EXPECT_EQ(twos_complement_20_hi(0x27, 0xA8, 0x01), 162433);
// }
// TEST(TwosComplement20Hi, NegativeLSBxF0) {
//     EXPECT_EQ(twos_complement_20_hi(0xF2, 0x46, 0x40), (0xF2464 - 1048576));
// }
// TEST(TwosComplement20Hi, NegativeLSBx0F) {
//     EXPECT_EQ(twos_complement_20_hi(0xD1, 0xDE, 0x0C), (0xD1DEC - 1048576));
// }
// TEST(TwosComplement20Hi, ZeroLSB) {
//     EXPECT_EQ(twos_complement_20_hi(0x12, 0xA8, 0x00), 0x12A80);
// }
// TEST(TwosComplement20Hi, Zero) {
//     EXPECT_EQ(twos_complement_20_hi(0x00, 0x00, 0x00), 0);
// }

// TEST(TwosComplement24, Positive) {
//     EXPECT_EQ(twos_complement_24(0x73, 0xE7, 0x59), 7595865);
// }
// TEST(TwosComplement24, Negative) {
//     EXPECT_EQ(twos_complement_24(0x89, 0xBC, 0x4B), (0x89BC4B - 16777216));
// }
// TEST(TwosComplement24, ZeroMSB) {
//     EXPECT_EQ(twos_complement_24(0x00, 0xD1, 0xD3), 0x00D1D3);
// }
// TEST(TwosComplement24, ZeroLSB) {
//     EXPECT_EQ(twos_complement_24(0xE1, 0x00, 0x8B), (0xE1008B - 16777216));
// }
// TEST(TwosComplement24, ZeroLSBX) {
//     EXPECT_EQ(twos_complement_24(0x79, 0x11, 0x00), 0x791100);
// }
// TEST(TwosComplement24, Zero) {
//     EXPECT_EQ(twos_complement_24(0x00, 0x00, 0x00), 0);
// }
// TEST(TwosComplement24, MaxInt24) {
//     EXPECT_EQ(twos_complement_24(0x80, 0x00, 0x00), -8388608);
// }

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     if (RUN_ALL_TESTS())
// 	;
// 	// Always return zero-code and allow PlatformIO to parse results
// 	return 0;
// }