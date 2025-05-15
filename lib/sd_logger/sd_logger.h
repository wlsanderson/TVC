#pragma once
#include <SD.h>
#include <sensor_packet.h>

class Logger {
public:
    void init(int CS_pin, int log_file_size, int page_size_bytes);
    void log_buffer(SensorPacket* buffer, int buffer_elements);
private:
    FsFile file;
    char log_name[24];
    size_t page_size;
    uint32_t write_position = 0;
};