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

    int lines = 0;
    int keywords = 0;
    int wifi_nets = 0;
    int cur = 0;

    bool is_csv = filename.endsWith(".csv");

    while(cur < (int)content.length()) {
        int next = content.indexOf('\n', cur);
        if (next == -1) next = content.length();
        String line = content.substring(cur, next);
        line.trim();

        if (line.length() > 0) {
            lines++;
            if (line.indexOf("password") != -1 || line.indexOf("user") != -1) keywords++;

            // If it's our scan_results.csv, count unique-ish entries
            if (is_csv && line.indexOf(",") != -1 && line.indexOf(":") != -1) {
                wifi_nets++;
            }
        }
        cur = next + 1;
    }

    if (is_csv && wifi_nets > 0) {
        console_printf("CSV Parse: Found %d logged networks.", wifi_nets);
    } else {
        console_printf("Result: %d lines, %d keywords found.", lines, keywords);
    }
}

void cmd_tools_crack(const String& params) {
    int spaceIdx = params.indexOf(' ');
    String target = (spaceIdx == -1) ? params : params.substring(0, spaceIdx);
    String dictFile = (spaceIdx == -1) ? "" : params.substring(spaceIdx + 1);

    console_printf("Target: %s", target.c_str());

    StorageManager& sm = StorageManager::getInstance();
    bool found = false;

    if (dictFile != "" && sm.isSDCardMounted() && sm.fileExists(dictFile.c_str())) {
        console_printf("Using SD dictionary: %s", dictFile.c_str());
        String content = sm.readFile(dictFile.c_str());
        int cur = 0;
        while(cur < (int)content.length()) {
            int next = content.indexOf('\n', cur);
            if (next == -1) next = content.length();
            String pass = content.substring(cur, next);
            pass.trim();
            if (pass.length() > 0) {
                console_printf("Trying: %s...", pass.c_str());
                if (target == pass) {
                    console_printf("SUCCESS! Found: %s", pass.c_str());
                    found = true;
                    break;
                }
                delay(100);
            }
            cur = next + 1;
        }
    } else {
        console_println("Using built-in demo dictionary...");
        const char* dict[] = {"admin", "123456", "password", "guest", "root"};
        for (int i = 0; i < 5; i++) {
            console_printf("Trying: %s...", dict[i]);
            delay(500);
            if (target == dict[i]) {
                console_printf("SUCCESS! Found: %s", dict[i]);
                found = true;
                break;
            }
        }
    }

    if (!found) {
        console_println("Cracking failed. Try a different dictionary.");
    }
}
