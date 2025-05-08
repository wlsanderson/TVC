#include "context.h"

TVCContext* TVCContext::instance = nullptr;

void TVCContext::init() {
    instance = this;
    // setup Serial, SPI, I2C, pinouts
    Serial.begin(9600);
    while (!Serial) {
        delay(50);
    }
    SPI.begin();
    Wire.begin();
    Wire.setClock(dps310_i2c_speed);

    // set interrupts
    pinMode(pressure_interrupt_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pressure_interrupt_pin), pressure_interrupt_handler, FALLING);
    
    pinMode(imu_interrupt_pin, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(imu_interrupt_pin), imu_interrupt_handler, CHANGE);

    imu.init();

    pressure_sensor.init();
}

void TVCContext::update() {
    if (pressure_temp_ready) {
        pressure_temp_ready = false;
        pressure_sensor.fetch(sensor_packet_queue);
    }

    if (imu_mag_ready) {
        imu_mag_ready = false;
        imu.fetch_imu_mag(sensor_packet_queue);
    }
    
    while (!sensor_packet_queue.empty()) {
        SensorPacket packet = sensor_packet_queue.front();
        Serial.println();
        Serial.print("P: ");
        Serial.println(packet.pressure);
        Serial.print("T: ");
        Serial.println(packet.temperature);

        Serial.print("Gyro X: ");
        Serial.println(packet.gyro_x);
        Serial.print("Gyro Y: ");
        Serial.println(packet.gyro_y);
        Serial.print("Gyro Z: ");
        Serial.println(packet.gyro_z);
        Serial.print("Accel X: ");
        Serial.println(packet.acc_x);
        Serial.print("Accel Y: ");
        Serial.println(packet.acc_y);
        Serial.print("Accel Z: ");
        Serial.println(packet.acc_z);
        Serial.print("Mag X: ");
        Serial.println(packet.mag_x);
        Serial.print("Mag Y: ");
        Serial.println(packet.mag_y);
        Serial.print("Mag Z: ");
        Serial.println(packet.mag_z);
        sensor_packet_queue.pop();
    }

}

void TVCContext::pressure_interrupt_handler() {
    if (instance) {
        instance->handle_pressure_interrupt();
    }
}

void TVCContext::handle_pressure_interrupt() {
    pressure_temp_ready = true;
}

void TVCContext::imu_interrupt_handler() {
    if (instance) {
        instance->handle_imu_interrupt();
    }
}

void TVCContext::handle_imu_interrupt() {
    imu_mag_ready = true;
}
