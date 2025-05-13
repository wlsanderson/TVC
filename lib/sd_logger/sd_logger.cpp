#include "sd_logger.h"
#include "sd_logger_constants.h"

void Logger::init(int CS_pin, int log_file_size, int page_size_bytes) {
    page_size = page_size_bytes;
    if (!SD.sdfs.begin(SdSpiConfig(CS_pin, DEDICATED_SPI, SD_SCK_MHZ(24)))) {
        Serial.println("SD card initialization failed");
        return;
    }
    SD.sdfs.ls();
    for (int i = 1; i <= max_log_files; i++) {
        snprintf(log_name, sizeof(log_name), "log_%02d.bin", i);
        if (!SD.sdfs.exists(log_name)) {
            break;
        }
    }

    file = SD.sdfs.open(log_name, O_WRITE | O_CREAT);
    if (file.preAllocate(log_file_size*1024*1024)) {
        Serial.print("Preallocated log file with "); Serial.print(log_file_size); Serial.println(" Mb");
    } else {
        Serial.println("Could not preallocate file");
    }
    delay(500); // preallocation can take a while
    file.close();
}

void Logger::log_buffer(SensorPacket* buffer, int buffer_length) {
    size_t data_size = sizeof(SensorPacket) * buffer_length;
    // Serial.println();
    // for (int i = 0; i < buffer_length; i++) {
    //     Serial.print(buffer[i].timestamp);
    //     Serial.print("\t");
    //     Serial.print(buffer[i].pressure);
    //     Serial.print("\t");
    //     Serial.print(buffer[i].gyro_x);
    //     Serial.print("\t");
    //     Serial.println(buffer[i].mag_x);
    // }
    if (data_size > page_size) {
        Serial.println("Logger: Buffer size is larger than intended page size!");
        return;
    }

    uint8_t buffer_block[page_size];
    memcpy(buffer_block, buffer, data_size);

    // pad remaining bytes
    if (data_size < page_size) {
        memset(buffer_block + data_size, 0xFF, page_size - data_size);
    }
    if (!file.open(log_name, O_WRITE)) {
        //Serial.println("Logger: Failed to open log file");
        return;
    }
    file.seekSet(write_position);
    if (file.write(buffer_block, page_size) != page_size) {
        Serial.println("Logger: Failed to write full block");
    }
    write_position += page_size;

    file.close();
}

