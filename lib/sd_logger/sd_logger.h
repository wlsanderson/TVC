#pragma once
#include <SD.h>
#include <sensor_packet.h>

class Logger {
public:
    Logger(int logger_cs_pin, size_t log_file_size, size_t log_page_size_bytes);
    void log_buffer(SensorPacket* buffer, int buffer_elements);
    void begin();
private:
    const int cs_pin;
    const size_t file_size;
    const size_t page_size;
    FsFile file;
    char log_name[24];
    size_t write_position = 0;
    
};