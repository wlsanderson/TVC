#include "spi_utils.h"


SPIUtils::SPIUtils(int spi_speed, uint8_t read_byte, int spi_mode, BitOrder bit_order, int CS_pin) {
    spi_speed = spi_speed;
    read_byte = read_byte;
    spi_mode = spi_mode;
    bit_order = bit_order;
    CS_pin = CS_pin;
}

uint8_t SPIUtils::read_register(uint8_t addr) {
    SPI.beginTransaction(SPISettings(spi_speed, bit_order, spi_mode));
    digitalWrite(CS_pin, LOW);
    SPI.transfer(addr | read_byte);
    uint8_t val = SPI.transfer(0x00);
    digitalWrite(CS_pin, HIGH);
    SPI.endTransaction();
    return val;
}
  
void SPIUtils::read_registers(uint8_t start_addr, uint8_t* buffer, int num_bytes) {
    SPI.beginTransaction(SPISettings(spi_speed, bit_order, spi_mode));
    digitalWrite(CS_pin, LOW);
    SPI.transfer(start_addr | read_byte);
  
    for (int i = 0; i < num_bytes; i++) {
        buffer[i] = SPI.transfer(0x00);
    }

    digitalWrite(CS_pin, HIGH);
    SPI.endTransaction();
}
  
void SPIUtils::write_register(uint8_t addr, uint8_t value) {
    SPI.beginTransaction(SPISettings(spi_speed, bit_order, spi_mode));
    digitalWrite(CS_pin, LOW);
    SPI.transfer(addr);
    SPI.transfer(value);
    digitalWrite(CS_pin, HIGH);
    SPI.endTransaction();
}