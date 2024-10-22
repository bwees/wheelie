#include <ArduinoOTA.h>

const char* ssid     = "fmbms";
const char* password = "ffm";

void ota_loop();
void ota_setup();
void ota_control(bool enable);