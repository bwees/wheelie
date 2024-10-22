#include <algorithm>
#include <cmath>
#include <cstdint>

#include "packet_parsers.h"

namespace {

inline int16_t int16FromNetworkOrder(const void* const p) {
  uint8_t* const charPointer = (uint8_t* const)p;
  return ((uint16_t)(*charPointer)) << 8 | *(charPointer + 1);
}

}  // namespace

int16_t batteryPercentageParser(Packet& p) {

  // 0x3 message is just one byte containing battery percentage.
  return *(int8_t*)p.data();
}

int16_t currentParser(Packet& p) {
  // 0x5 message encodes current as signed int16.
  // The scaling factor (tested on a Pint) seems to be 0.055
  // i.e. 1 in the data message below corresponds to 55 milliamps.
  return int16FromNetworkOrder(p.data()) * CURRENT_SCALER;
}

uint32_t bmsSerialParser(Packet& p) {
  // 0x6 message has the BMS serial number encoded inside of it.
  // It is the last seven digits from the sticker on the back of the BMS.
  uint32_t captured_serial;
  for (int i = 0; i < 4; i++) {
    captured_serial |= p.data()[i] << (8 * (3 - i));
  }

  return captured_serial;
}

void cellVoltageParser(Packet& p, uint16_t* cells) {
  // The data in this packet is 16 int16-s. First 15 of them is
  // individual cell voltages in millivolts. The last value is mysterious.
  const uint8_t* const data = p.data();
  uint16_t total_voltage = 0;

  for (int i = 0; i < 15; i++) {
    cells[i] = int16FromNetworkOrder(data + (i << 1));
  }
}

void temperatureParser(Packet& p, int8_t* temps) {
  int8_t* const temperatures = (int8_t*)p.data();
  for (int i = 0; i < 5; i++) {
    temps[i] = temperatures[i];
  }
}

uint8_t bmsStatusParser(Packet& p) {
  return p.data()[0];
}
