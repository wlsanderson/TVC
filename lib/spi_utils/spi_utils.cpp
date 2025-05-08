#include "spi_utils.h"


void SPIUtils::init(int spi_speed_hz, uint8_t spi_read_byte, int mode, BitOrder spi_bit_order, int spi_CS_pin) {
    this->spi_speed = spi_speed_hz;
    this->read_byte = spi_read_byte;
    this->spi_mode = mode;
    this->bit_order = spi_bit_order;
    this->CS_pin = spi_CS_pin;
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