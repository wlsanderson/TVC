#pragma once
#include <stdint.h>
#include <Arduino.h>
#include <SPI.h>

/**
 * this allows ArduinoFake to be compatible with Arduino's BitOrder type, because ArduinoFake
 * defines MSBFIRST, LSBFIRST, and some other stuff as preprocessor macros, and doesn't recognize
 * the BitOrder enum.
 */
#ifdef MSBFIRST
#define BitOrder int
#endif

class SPIUtils {
    public:
        uint8_t read_byte = 0x00;
        int CS_pin;
        int spi_speed;
        BitOrder bit_order;
        int spi_mode;

        void begin(int spi_speed, uint8_t read_byte, int spi_mode, BitOrder bit_order, int CS_pin);

        uint8_t read_register(uint8_t addr);
        void read_registers(uint8_t start_addr, uint8_t* buffer, int num_bytes);
        void write_register(uint8_t addr, uint8_t value);
};
