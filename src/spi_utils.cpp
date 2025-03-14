#include "constants.h"

void initSPI(uint8_t pin) {
    SPI.begin();
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
}

uint8_t readRegister(uint8_t addr, uint8_t pin) {
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(pin, LOW);
    SPI.transfer(addr | READ);
    uint8_t val = SPI.transfer(0x00);
    digitalWrite(pin, HIGH);
    SPI.endTransaction();
    return val;
}
  
void readRegisters(uint8_t start_addr, uint8_t* buffer, int num_bytes, uint8_t pin) {
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(pin, LOW);
    SPI.transfer(start_addr | READ);
  
    for (int i = 0; i < num_bytes; i++) {
        buffer[i] = SPI.transfer(0x00);
    }

    digitalWrite(pin, HIGH);
    SPI.endTransaction();
}
  
void writeRegister(uint8_t addr, uint8_t value, uint8_t pin) {
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(pin, LOW);
    SPI.transfer(addr);
    SPI.transfer(value);
    digitalWrite(pin, HIGH);
    SPI.endTransaction();
}