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

        SPIUtils(int spi_speed, uint8_t read_byte, int spi_mode, BitOrder bit_order, int CS_pin);
         
        // If not using a virtual destructor, leads to undefined behavior if the base class of
        // SPIUtils is deleted when SPIUtils has a derived class (like MockSPIUtils)
        virtual ~SPIUtils() {}

        virtual uint8_t read_register(uint8_t addr);
        virtual void read_registers(uint8_t start_addr, uint8_t* buffer, int num_bytes);
        virtual void write_register(uint8_t addr, uint8_t value);
};
