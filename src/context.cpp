#include "context.h"
#include "constants.h"
#include "spi_utils.h"

void TVCContext::init() {
    // setup Serial, SPI, pinouts
    Serial.begin(9600);
    while (!Serial) {
        delay(50);
    }
    SPI.begin();
    pinMode(CS_PIN_DPS310, OUTPUT);

    // initialize sensors
    pressure_sensor.init();
}

void TVCContext::update() {
    pressure_sensor.fetch();
}