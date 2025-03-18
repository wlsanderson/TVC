#include <tvc_utils.h>
#include <unity.h>


void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_twos_complement_12_hi() {
    // positve value, lsb_4 is on 0xF0
    TEST_ASSERT_EQUAL(1140,twos_complement_12_hi(0b01000111, 0b01000000));
    // positive value, lsb_4 is on 0x0F
    TEST_ASSERT_EQUAL(71,twos_complement_12_hi(0b00000100, 0b00000111));
    // negative value, lsb_4 is on 0xF0
    TEST_ASSERT_EQUAL((0b110101001010 - 4096),twos_complement_12_hi(0b11010100, 0b10100000));
    // negative value, lsb_4 is on 0x0F
    TEST_ASSERT_EQUAL((0b101101001111 - 4096),twos_complement_12_hi(0b10110100, 0b00001111));
}

void test_twos_complement_12_lo() {
    // positve value, msb_4 is on 0xF0
    TEST_ASSERT_EQUAL(1095,twos_complement_12_lo(0b01000000, 0b01000111));
    // positive value, msb_4 is on 0x0F
    TEST_ASSERT_EQUAL(1892,twos_complement_12_lo(0b00000111, 0b01100100));
    // negative value, msb_4 is on 0xF0
    TEST_ASSERT_EQUAL((0b101011010100 - 4096),twos_complement_12_lo(0b10100000, 0b11010100));
    // negative value, msb_4 is on 0x0F
    TEST_ASSERT_EQUAL((0b111110110100 - 4096),twos_complement_12_lo(0b00001111, 0b10110100));
}

void test_twos_complement_16() {
    // positive value
    TEST_ASSERT_EQUAL(30162,twos_complement_16(0b01110101, 0b11010010));
    // negative value
    TEST_ASSERT_EQUAL((0b1010111100010100 - 65536), twos_complement_16(0b10101111, 0b00010100));
}

void test_twos_complement_20_hi() {
    // positve value, lsb_4 is on 0xF0
    TEST_ASSERT_EQUAL(326346,twos_complement_20_hi(0x4F, 0xAC, 0xA0));
    // positive value, lsb_4 is on 0x0F
    TEST_ASSERT_EQUAL(162433,twos_complement_20_hi(0x27, 0xA8, 0x01));
    // negative value, lsb_4 is on 0xF0
    TEST_ASSERT_EQUAL((0xF2464 - 1048576),twos_complement_20_hi(0xF2, 0x46, 0x40));
    // negative value, lsb_4 is on 0x0F
    TEST_ASSERT_EQUAL((0xD1DEC - 1048576),twos_complement_20_hi(0xD1, 0xDE, 0x0C));
}

void test_twos_complement_20_lo() {
    // positve value, msb_4 is on 0xF0
    TEST_ASSERT_EQUAL(479148,twos_complement_20_lo(0x70, 0x4F, 0xAC));
    // positive value, msb_4 is on 0x0F
    TEST_ASSERT_EQUAL(75688,twos_complement_20_lo(0x01, 0x27, 0xA8));
    // negative value, msb_4 is on 0xF0
    TEST_ASSERT_EQUAL((0xBF246 - 1048576),twos_complement_20_lo(0xB0, 0xF2, 0x46));
    // negative value, msb_4 is on 0x0F
    TEST_ASSERT_EQUAL((0xCD1DE - 1048576),twos_complement_20_lo(0x0C, 0xD1, 0xDE));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_twos_complement_12_hi);
    RUN_TEST(test_twos_complement_12_lo);
    RUN_TEST(test_twos_complement_16);
    RUN_TEST(test_twos_complement_20_hi);
    RUN_TEST(test_twos_complement_20_lo);
    UNITY_END();

    return 0;
}