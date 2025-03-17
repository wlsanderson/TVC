#include "constants.h"


uint8_t spi_read_register(uint8_t addr, uint8_t pin) {
    SPI.beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE0));
    digitalWrite(pin, LOW);
    SPI.transfer(addr | READ);
    uint8_t val = SPI.transfer(0x00);
    digitalWrite(pin, HIGH);
    SPI.endTransaction();
    return val;
}
  
void spi_read_registers(uint8_t start_addr, uint8_t* buffer, int num_bytes, uint8_t pin) {
    SPI.beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE0));
    digitalWrite(pin, LOW);
    SPI.transfer(start_addr | READ);
  
    for (int i = 0; i < num_bytes; i++) {
        buffer[i] = SPI.transfer(0x00);
    }

    digitalWrite(pin, HIGH);
    SPI.endTransaction();
}
  
void spi_write_register(uint8_t addr, uint8_t value, uint8_t pin) {
    SPI.beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE0));
    digitalWrite(pin, LOW);
    SPI.transfer(addr);
    SPI.transfer(value);
    digitalWrite(pin, HIGH);
    SPI.endTransaction();
}