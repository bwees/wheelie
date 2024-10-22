#include "ota.h"

bool enabled = false;

void ota_setup() {
    ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {  // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });
}

void ota_control(bool enable) {
    if (enable) {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(ssid, password);
        Serial.print("[+] AP Created with IP Gateway ");
        Serial.println(WiFi.softAPIP());

        ArduinoOTA.begin();
    } else {
        WiFi.disconnect();
        ArduinoOTA.end();
    }

    enabled = enable;
}

void ota_loop() {
    if (enabled) ArduinoOTA.handle();
}