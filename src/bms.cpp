#include <bms_relay.h>

#include "bms.h"
#include "canbus.h"

BmsRelay* relay;

void bms_setup(HardwareSerial* serial) {
    relay = new BmsRelay([serial]() { return serial->read(); }, // Read from bms serial
                       [](uint8_t b) {}, // we dont care about transmitting
                       millis);

    relay->addReceivedPacketCallback([](BmsRelay *, Packet *packet) {
        switch(packet->getType()) {
            case PACKET_CELL_VOLTAGE:
                can_send_cell_voltage(packet);
        }
    });
}

void bms_loop() {
    relay->loop();
}