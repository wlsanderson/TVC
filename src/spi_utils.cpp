#include "constants.h"

void initSPI() {
    SPI.begin();
    pinMode(CS_PIN, OUTPUT);
    digitalWrite(CS_PIN, HIGH);
}

uint8_t readRegister(uint8_t addr) {
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(addr | READ);
    uint8_t val = SPI.transfer(0x00);
    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
    return val;
}
  
void readRegisters(uint8_t start_addr, uint8_t* buffer, int num_bytes) {
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(start_addr | READ);
  
    for (int i = 0; i < num_bytes; i++) {
        buffer[i] = SPI.transfer(0x00);
    }

    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
}
  
void writeRegister(uint8_t addr, uint8_t value) {
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(addr);
    SPI.transfer(value);
    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
}