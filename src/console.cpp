#include "console.h"

#include "commands/cmd_sys.h"
#include "commands/cmd_wifi.h"
#include "commands/cmd_hid.h"
#include "commands/cmd_multitask.h"
#include "commands/cmd_power.h"
#include "commands/cmd_storage.h"
#include "commands/cmd_ble.h"
#include "commands/cmd_config.h"
#include "commands/cmd_tools.h"

#include "multitask.h"
#include "power_manager.h"
#include "storage_manager.h"
#include "ble_manager.h"
#include "config_manager.h"
#include <stdarg.h>

static const int MAX_LINES = 32;
static String consoleLines[MAX_LINES];
static int lineCount = 0;

void console_init() {
    MultitaskManager::getInstance().init();
    PowerManager::getInstance().init();
    StorageManager::getInstance().init();
    ConfigManager::getInstance().init();
}

void console_println(const String& line) {
    Serial.println(line);
    if (lineCount < MAX_LINES) {
        consoleLines[lineCount++] = line;
    } else {
        for (int i = 1; i < MAX_LINES; ++i) {
            consoleLines[i - 1] = consoleLines[i];
        }
        consoleLines[MAX_LINES - 1] = line;
    }
}

void console_printf(const char* format, ...) {
    char loc_buf[128];
    va_list arg;
    va_start(arg, format);
    vsnprintf(loc_buf, sizeof(loc_buf), format, arg);
    va_end(arg);
    console_println(String(loc_buf));
}

void console_process_line(const String& line) {

    // -------------------------
    // HELP
    // -------------------------
    if (line == "help") {
        console_println("\n=== Available Commands ===");

        console_println("System:");
        console_println("  sysinfo                - System information");
        console_println("  help                   - Show this help");

        console_println("\nWiFi:");
        console_println("  wifi scan              - Scan for networks");

        console_println("\nHID (Human Interface):");
        console_println("  hid test               - Test keyboard/buttons");
        console_println("  hid info               - Keyboard information");
        console_println("  hid mouse              - Mouse emulation info");
        console_println("  hid gamepad            - Gamepad test");

        console_println("\nMultitasking:");
        console_println("  task list              - List active tasks");

        console_println("\nPower Management:");
        console_println("  power info             - Power information");
        console_println("  power battery          - Battery status");
        console_println("  power mode             - Power modes");
        console_println("  power sleep            - Enter sleep mode");

        console_println("\nStorage:");
        console_println("  storage info           - Storage information");
        console_println("  storage ls             - List SD card contents");

        console_println("\nBluetooth:");
        console_println("  ble start              - Start BLE advertising");
        console_println("  ble stop               - Stop BLE advertising");
        console_println("  ble info               - BLE information");

        console_println("\nConfiguration:");
        console_println("  config show            - Show configuration");
        console_println("  config save            - Save configuration");
        console_println("  config reset           - Reset to defaults");

        console_println("\nAdvanced Tools:");
        console_println("  scan bssid             - Detailed WiFi scan");
        console_println("  parse <file>           - Parse log file");
        console_println("  crack <target>         - Simulation/Test tool");

        console_println("===========================\n");
        return;
    }

    // -------------------------
    // TOOLS
    // -------------------------
    if (line == "scan bssid") {
        cmd_wifi_scan_details();
        return;
    }

    if (line.startsWith("parse ")) {
        cmd_tools_parse(line.substring(6));
        return;
    }

    if (line.startsWith("crack ")) {
        cmd_tools_crack(line.substring(6));
        return;
    }

    // -------------------------
    // SYSTEM
    // -------------------------
    if (line == "sysinfo") {
        cmd_sysinfo();
        return;
    }

    // -------------------------
    // WIFI
    // -------------------------
    if (line == "wifi scan") {
        cmd_wifi_scan();
        return;
    }

    // -------------------------
    // HID
    // -------------------------
    if (line == "hid test") {
        cmd_hid_test();
        return;
    }

    if (line == "hid info") {
        cmd_hid_keyboard_info();
        return;
    }

    if (line == "hid mouse") {
        cmd_hid_mouse_emulate();
        return;
    }

    if (line == "hid gamepad") {
        cmd_hid_gamepad_test();
        return;
    }

    // -------------------------
    // MULTITASK
    // -------------------------
    if (line == "task list") {
        cmd_task_list();
        return;
    }

    // -------------------------
    // POWER
    // -------------------------
    if (line == "power info") {
        cmd_power_info();
        return;
    }

    if (line == "power battery") {
        cmd_power_battery();
        return;
    }

    if (line == "power mode") {
        cmd_power_mode();
        return;
    }

    if (line == "power sleep") {
        cmd_power_sleep();
        return;
    }

    // -------------------------
    // STORAGE
    // -------------------------
    if (line == "storage info") {
        cmd_storage_info();
        return;
    }

    if (line == "storage ls") {
        cmd_storage_ls();
        return;
    }

    // -------------------------
    // BLE
    // -------------------------
    if (line == "ble start") {
        cmd_ble_start();
        return;
    }

    if (line == "ble stop") {
        cmd_ble_stop();
        return;
    }

    if (line == "ble info") {
        cmd_ble_info();
        return;
    }

    // -------------------------
    // CONFIG
    // -------------------------
    if (line == "config show") {
        cmd_config_show();
        return;
    }

    if (line == "config save") {
        cmd_config_save();
        return;
    }

    if (line == "config reset") {
        cmd_config_reset();
        return;
    }

    // -------------------------
    // UNKNOWN
    // -------------------------
    console_println("Unknown command. Type 'help' for available commands.");
}

// These are needed by main.cpp for redraw
int console_get_line_count() { return lineCount; }
String console_get_line(int index) { if (index >= 0 && index < lineCount) return consoleLines[index]; return ""; }