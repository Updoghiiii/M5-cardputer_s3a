#include "cmd_wifi.h"
#include "../console.h"
#include <WiFi.h>
#include <Arduino.h>

void cmd_wifi_scan() {
    console_println("=== WiFi Scan ===");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);
    delay(100);

    console_println("Scanning...");
    int n = WiFi.scanNetworks(false, true);

    if (n <= 0) {
        console_println("No networks found");
        return;
    }

    console_printf("Found %d networks", n);

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

        console_printf(
            "%2d: %-15s %4d %2d %s",
            i,
            ssid.substring(0, 15).c_str(),
            rssi,
            channel,
            encStr
        );
    }

    console_println("\nScan complete");
}