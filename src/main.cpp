#include <Arduino.h>
#include "context.h"

TVCContext context;

void setup() {
    int error_status = context.begin();
    if (error_status) {
        Serial.println("Program stopped");
        while (1) {}
    }
}

void loop() {
    context.update();
}
