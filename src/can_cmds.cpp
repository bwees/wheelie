#include <CAN.h>

#include "can_cmds.h"
#include "buffer.h"
#include "config.h"

// Flushes the CAN read buffer since we don't need it.
void can_flush_read() {
  if(CAN.parsePacket()){
    for(int i = 0; i<CAN.packetDlc(); i++){
        CAN.read();
    }
  }
}

// Function to send the cell voltage over CAN
void can_send_cell_voltage(int cell_num, float voltage){

    int32_t send_index = 0;
    uint8_t buffer[8];

    // Append the cell offset and the total number of cells (assuming 1 cell for this example)
    buffer[send_index++] = cell_num;       // Cell offset we are starting at
    buffer[send_index++] = NUM_CELLS;      // Number of cells total in the pack

    // Append the voltage as a scaled 16-bit float
    buffer_append_float16(buffer, voltage, 1e3, &send_index);

    // Set up the CAN frame
    uint32_t msgID = CAN_ID | (CAN_PACKET_BMS_V_CELL << 8);  // Construct message ID

    // send index will be length of message
    CAN.beginExtendedPacket(msgID, send_index);
    CAN.write(buffer, send_index);
    CAN.endPacket();
}
