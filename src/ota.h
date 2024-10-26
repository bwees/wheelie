#pragma once

extern const char* ssid;
extern const char* password;
extern bool otaEnabled;


void ota_loop();
void ota_setup();
void ota_control(bool enable);