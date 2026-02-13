#include "cmd_tools.h"
#include "../console.h"
#include "../storage_manager.h"
#include <WiFi.h>

void cmd_wifi_scan_details() {
    console_println("=== Detailed WiFi Scan ===");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    int n = WiFi.scanNetworks();
    if (n == 0) {
        console_println("No networks found.");
    } else {
        console_printf("Found %d networks:", n);
        for (int i = 0; i < n; ++i) {
            console_printf("%d: %s", i + 1, WiFi.SSID(i).c_str());
            console_printf("  BSSID: %s", WiFi.BSSIDstr(i).c_str());
            console_printf("  RSSI: %d dBm | CH: %d", WiFi.RSSI(i), WiFi.channel(i));
            console_println("------------------");
            delay(10);
        }
    }
}

void cmd_tools_parse(const String& filename) {
    StorageManager& sm = StorageManager::getInstance();
    if (!sm.isSDCardMounted()) {
        console_println("Error: SD card not mounted.");
        return;
    }

    String content = sm.readFile(filename.c_str());
    if (content == "") {
        console_println("Error: Could not read file or file empty.");
        return;
    }

    console_printf("Parsing %s...", filename.c_str());
    // Simple parser: count lines and find keywords
    int lines = 0;
    int keywords = 0;
    int cur = 0;
    while(cur < (int)content.length()) {
        int next = content.indexOf('\n', cur);
        if (next == -1) next = content.length();
        String line = content.substring(cur, next);
        lines++;
        if (line.indexOf("password") != -1 || line.indexOf("user") != -1) keywords++;
        cur = next + 1;
    }

    console_printf("Result: %d lines, %d interesting entries found.", lines, keywords);
}

void cmd_tools_crack(const String& target) {
    console_printf("Starting simulation on: %s", target.c_str());
    console_println("Running dictionary attack...");

    const char* dict[] = {"admin", "123456", "password", "guest", "root"};
    bool found = false;

    for (int i = 0; i < 5; i++) {
        console_printf("Trying: %s...", dict[i]);
        delay(500);
        if (target == dict[i]) {
            console_printf("SUCCESS! Password found: %s", dict[i]);
            found = true;
            break;
        }
    }

    if (!found) {
        console_println("Cracking failed. Try a different dictionary.");
    }
}
