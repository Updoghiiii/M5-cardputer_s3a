#include "cmd_wifi.h"
#include <WiFi.h>
#include <Arduino.h>

void cmd_wifi_scan() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    int n = WiFi.scanNetworks();
    Serial.printf("Found %d networks\n", n);

    for (int i = 0; i < n; i++) {
        Serial.printf("%d: %s (%d dBm)\n",
                      i,
                      WiFi.SSID(i).c_str(),
                      WiFi.RSSI(i));
    }
}