#include <ESP32-TWAI-CAN.hpp>

#include "can_cmds.h"
#include "config.h"
#include "packet_parsers.h"

void buffer_append_int16(uint8_t* buffer, int16_t number, int32_t *index) {
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}
void buffer_append_float16(uint8_t* buffer, float number, float scale, int32_t *index) {
    buffer_append_int16(buffer, (int16_t)(number * scale), index);
}

// Handles incoming CAN messages
void can_loop() {
    CanFrame rxFrame;

    // read a frame
    // if no frame, we don't need to check it
    if (!ESP32Can.readFrame(rxFrame)) return;

    // is it an OTA AP enable/disable?
    if (rxFrame.identifier == 0x2A685A) {
        if (rxFrame.data[0]) {
            // enable AP
        } else {
            // disable AP
        }
    }
}

// Function to send the cell voltage over CAN
void can_send_cell_voltage(Packet *packet) {

    uint16_t cells[NUM_CELLS];
    cellVoltageParser(*packet, cells);
    
    int current_cell = 0;
    int cell_max = NUM_CELLS-1;
    while (current_cell < cell_max) {
        int32_t send_index = 0;
        uint8_t buffer[8];

        // Append the cell offset and the total number of cells (assuming 1 cell for this example)
        buffer[send_index++] = current_cell;       // Cell offset we are starting at
        buffer[send_index++] = NUM_CELLS;      // Number of cells total in the pack

        if (current_cell < cell_max) {
            buffer_append_float16(buffer, cells[current_cell++], 1e3, &send_index);
        }
        if (current_cell < cell_max) {
            buffer_append_float16(buffer, cells[current_cell++], 1e3, &send_index);
        }
        if (current_cell < cell_max) {
            buffer_append_float16(buffer, cells[current_cell++], 1e3, &send_index);
        }

        transmit_can(CAN_ID | (CAN_PACKET_BMS_V_CELL << 8), buffer, send_index);
    }
}


void transmit_can(uint32_t id, uint8_t* buffer, int length) {
    // construct message
    CanFrame frame;
    frame.identifier = id;
    frame.extd = true;
    frame.data_length_code = length;

    // copy in data
    for (int i=0; i< length; i++) {
        frame.data[i] = buffer[i];
    }

    ESP32Can.writeFrame(frame);
}
