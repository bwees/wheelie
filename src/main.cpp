#include <Arduino.h>
#include <CAN.h>
#include <can_cmds.h>

void setup() {
    // open CAN at 500kbps
    if (!CAN.begin(500E3)) {
        Serial.println("Starting CAN failed!");
        while (1);
    }
}


void loop() {
    can_flush_read();
    
    can_send_cell_voltage(1, 3.6);
}