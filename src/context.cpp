#include "context.h"

TVCContext* TVCContext::instance = nullptr;

TVCContext::TVCContext()
    : pressure_sensor{0x77},
    imu{0x6B, 0x1E},
    logger{sd_card_cs_pin, log_file_size, log_page_size_bytes},
    ukf{alpha_coefficient, beta_coefficient, kappa_coefficient} {
    
    instance = this;
}

int TVCContext::begin() {
    // setup Serial, SPI, I2C, pinouts
    Serial.begin(9600);
    while (!Serial) {
        delay(50);
    }
    SPI.begin();
    Wire.begin();
    Wire.setClock(i2c_clock_speed);

    // set interrupts
    pinMode(pressure_interrupt_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pressure_interrupt_pin), pressure_interrupt_handler, FALLING);
    pinMode(imu_interrupt_pin, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(imu_interrupt_pin), imu_interrupt_handler, RISING);

    if (imu.begin()) {
        Serial.println("Context: IMU Initialization Failed!");
        return 1;
    } else Serial.println("IMU Initialization Successful!");
    pressure_sensor.begin();
    logger.begin();
    
    switch (first_fetch()) {
        case 0:
            break;
        case 1:
            Serial.println("Fetching first IMU packet failed!");
            return 1;
        case 2:
            Serial.println("Fetching first pressure/temp packet failed!");
            return 1;
        default:
            Serial.println("First fetch had an unexpected error!");
            return 1;
    }

    if (ukf.begin(first_fetch_buffer[1].pressure, first_fetch_buffer[0].acc_x, first_fetch_buffer[0].acc_y, first_fetch_buffer[0].acc_z, first_fetch_buffer[0].mag_x, first_fetch_buffer[0].mag_y, first_fetch_buffer[0].mag_z)) {
        Serial.println("UKF startup failed!");
        return 1;
    }
    return 0;

}

void TVCContext::update() {
    if (pressure_temp_ready) {
        pressure_temp_ready = false;
        
        if (filling_buffer_1) {
            next_packet_index = pressure_sensor.fetch(sensor_packet_buffer_1, next_packet_index);
        } else {
            next_packet_index = pressure_sensor.fetch(sensor_packet_buffer_2, next_packet_index);
        }
        
        filling_buffer_1 = determine_buffer(next_packet_index);
    }

    if (imu_mag_ready) {

        imu_mag_ready = false;
        
        if (filling_buffer_1) {
            next_packet_index = imu.fetch_imu_mag(sensor_packet_buffer_1, next_packet_index);
        } else {
            next_packet_index = imu.fetch_imu_mag(sensor_packet_buffer_2, next_packet_index);
        }

        filling_buffer_1 = determine_buffer(next_packet_index);

    }
    if (ready_to_log) {
    
        if (filling_buffer_1) {
            // has already started filling buffer 1, so log the full buffer (buffer 2)
            logger.log_buffer(sensor_packet_buffer_2, log_page_size_bytes / packet_size_bytes);
            //Serial.println("logged buffer 2");
        } else {
            // already stared filling buffer 2, so log buffer 1
            logger.log_buffer(sensor_packet_buffer_1, log_page_size_bytes / packet_size_bytes);
            //Serial.println("logged buffer 1");
        }
        Serial.println("logged");
        ready_to_log = false;
    }
}

int TVCContext::first_fetch() {
    delay(500); // wait to ensure both sensors are ready
    int idx = imu.fetch_imu_mag(first_fetch_buffer, 0);
    if (idx != 1) {
        return 1;
    }
    idx = pressure_sensor.fetch(first_fetch_buffer, idx);
    if (idx != 2) {
        return 2;
    }
    return 0;
}

bool TVCContext::determine_buffer(size_t next_index) {
    // if the next index is within bounds of array size, then use the buffer that isnt logging
    if (next_index < (log_page_size_bytes / packet_size_bytes)) {
        return filling_buffer_1;
    }
    // else, buffer is filled, reset index and start filling next
    next_packet_index = 0;
    ready_to_log = true;
    return !filling_buffer_1;
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
    if (!imu.hold_interrupt) {
        imu_mag_ready = true;
    }
}
