#include "context.h"
#include "constants.h"
#include "spi_utils.h"

void TVCContext::init() {
    // setup Serial, SPI, I2C, pinouts
    Serial.begin(9600);
    while (!Serial) {
        delay(50);
    }
    SPI.begin();
    Wire.begin();
    Wire.setClock(i2c_speed);
    // pressure sensor
    pinMode(CS_PIN_DPS310, OUTPUT);
    digitalWrite(CS_PIN_DPS310, LOW);
    delay(10);
    digitalWrite(CS_PIN_DPS310, HIGH);


    delay(100);

    // initialize sensors
    imu.init();
    pressure_sensor.init();

    delay(100);
    imu.fetch(sensor_packet_queue);
    
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