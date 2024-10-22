#include "packet.h"

// Pint BMS current scale value
#define CURRENT_SCALER 55;

uint8_t bmsStatusParser(Packet& p);
uint32_t bmsSerialParser(Packet& p);
int16_t currentParser(Packet& p);
int16_t batteryPercentageParser(Packet& p);
void cellVoltageParser(Packet& p, uint16_t* cells);
void temperatureParser(Packet& p, int8_t* temps);