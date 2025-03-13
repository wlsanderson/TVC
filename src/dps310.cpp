#include "dps310.h"
#include "constants.h"
#include "spi_utils.h"


DPS310::DPS310() {
    Serial.print("DPS310 ID: ");
    Serial.println(readRegister(ID), HEX);
}