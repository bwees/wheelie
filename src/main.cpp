#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>
#include <HardwareSerial.h>

#include "config.h"
#include "bms.h"
#include "ota.h"
#include "canbus.h"

HardwareSerial BMSSerial(0);

void setup() {
    ESP32Can.setPins(CAN_TX, CAN_RX);

    // open CAN at 500kbps
    if (!ESP32Can.begin(ESP32Can.convertSpeed(500))) {
        Serial.println("Starting CAN failed!");
        while (1);
    }

    // fudged RS485 comms
    BMSSerial.begin(115200, SERIAL_8N1, BMS_RX, -1);

    bms_setup(&BMSSerial);
    ota_setup();
}


void loop() {
    bms_loop();
    can_loop();
    ota_loop();

    delay(10);
}