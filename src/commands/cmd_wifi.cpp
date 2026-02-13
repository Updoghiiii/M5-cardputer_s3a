#include "cmd_wifi.h"
#include <WiFi.h>
#include <Arduino.h>

void cmd_wifi_scan() {
    Serial.println("=== WiFi Scan ===");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);
    delay(100);

    Serial.println("Scanning...");
    int n = WiFi.scanNetworks(false, true);

    if (n <= 0) {
        Serial.println("No networks found");
        return;
    }

    Serial.printf("Found %d networks\n\n", n);

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (WiFi.RSSI(j) > WiFi.RSSI(i)) {
                WiFi.scanSwap(i, j);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        String ssid = WiFi.SSID(i);
        int32_t rssi = WiFi.RSSI(i);
        uint8_t enc = WiFi.encryptionType(i);
        int32_t channel = WiFi.channel(i);

        const char* encStr =
            (enc == WIFI_AUTH_OPEN) ? "OPEN" :
            (enc == WIFI_AUTH_WEP) ? "WEP" :
            (enc == WIFI_AUTH_WPA_PSK) ? "WPA" :
            (enc == WIFI_AUTH_WPA2_PSK) ? "WPA2" :
            (enc == WIFI_AUTH_WPA_WPA2_PSK) ? "WPA/WPA2" :
            (enc == WIFI_AUTH_WPA2_ENTERPRISE) ? "WPA2-EAP" :
            "UNKNOWN";

        Serial.printf(
            "%2d: %-20s  RSSI: %4d dBm  CH: %2d  ENC: %s\n",
            i,
            ssid.c_str(),
            rssi,
            channel,
            encStr
        );
    }

    Serial.println("\nScan complete");
}