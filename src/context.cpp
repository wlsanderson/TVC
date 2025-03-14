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
    pressure_sensor.fetch(sensor_packet_queue);
    while (!sensor_packet_queue.empty()) {
        SensorPacket packet = sensor_packet_queue.front();
        Serial.print("P: ");
        Serial.println(packet.pressure);
        Serial.print("T: ");
        Serial.println(packet.temperature);
        sensor_packet_queue.pop();
    }
}