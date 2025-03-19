#pragma once
#include <stdint.h>
#include <Arduino.h>
#include <SPI.h>



class SPIUtils {
    public:
        uint8_t read_byte = 0x00;
        int CS_pin;
        int spi_speed;
        int bit_order = MSBFIRST;
        int spi_mode = SPI_MODE0;

        SPIUtils(int spi_speed, uint8_t read_byte, int spi_mode, int bit_order, int CS_pin);

        virtual uint8_t read_register(uint8_t addr);
        virtual void read_registers(uint8_t start_addr, uint8_t* buffer, int num_bytes);
        virtual void write_register(uint8_t addr, uint8_t value);
};
