#include "sd_logger.h"
#include "sd_logger_constants.h"

Logger::Logger(int logger_cs_pin, size_t log_file_size, size_t log_page_size_bytes)
    : cs_pin {logger_cs_pin}, file_size {log_file_size}, page_size {log_page_size_bytes} {}

void Logger::begin() {
    if (!SD.sdfs.begin(SdSpiConfig(cs_pin, DEDICATED_SPI, SD_SCK_MHZ(24)))) {
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
    if (file.preAllocate(file_size * 1024 * 1024)) {
        Serial.print("Preallocated log file with "); Serial.print(file_size); Serial.println(" Mb");
    } else {
        Serial.println("Could not preallocate file");
    }
    delay(200); // preallocation can take a while
    file.close();
}

void Logger::log_buffer(SensorPacket* buffer, int buffer_length) {

    size_t data_size = sizeof(SensorPacket) * buffer_length;

    if (data_size > page_size) {
        Serial.println("Logger: Buffer size is larger than intended page size!");
        return;
    }

    uint8_t buffer_block[page_size];
    memcpy(buffer_block, buffer, data_size);

    // pad remaining bytes
    if (data_size < page_size) {
        Serial.println("buffering");
        memset(buffer_block + data_size, 0xFF, page_size - data_size);
    }
    if (!file.open(log_name, O_WRITE)) {
        Serial.println("Logger: Failed to open log file");
        return;
    }
    file.seekSet(write_position);
    if (file.write(buffer_block, page_size) != page_size) {
        Serial.println("Logger: Failed to write full block");
    }
    write_position += page_size;

    file.close();
}

