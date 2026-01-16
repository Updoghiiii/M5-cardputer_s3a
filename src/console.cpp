#include "console.h"
#include "commands/cmd_sys.h"
#include "commands/cmd_wifi.h"
#include "commands/cmd_hid.h"
#include "commands/cmd_multitask.h"
#include "commands/cmd_power.h"
#include "commands/cmd_storage.h"
#include "commands/cmd_ble.h"
#include "commands/cmd_config.h"
#include "multitask.h"
#include "power_manager.h"
#include "storage_manager.h"
#include "ble_manager.h"
#include "config_manager.h"

void console_init() {
    MultitaskManager::getInstance().init();
    PowerManager::getInstance().init();
    StorageManager::getInstance().init();
    ConfigManager::getInstance().init();
}

void console_process_line(const String& line) {
    if (line == "help") {
        Serial.println("\n=== Available Commands ===");
        Serial.println("System:");
        Serial.println("  sysinfo                - System information");
        Serial.println("  help                   - Show this help");
        Serial.println("\nWiFi:");
        Serial.println("  wifi scan              - Scan for networks");
        Serial.println("\nHID (Human Interface):");
        Serial.println("  hid test               - Test keyboard/buttons");
        Serial.println("  hid info               - Keyboard information");
        Serial.println("  hid mouse              - Mouse emulation info");
        Serial.println("  hid gamepad            - Gamepad test");
        Serial.println("\nMultitasking:");
        Serial.println("  task list              - List active tasks");
        Serial.println("\nPower Management:");
        Serial.println("  power info             - Power information");
        Serial.println("  power battery          - Battery status");
        Serial.println("  power mode             - Power modes");
        Serial.println("  power sleep            - Enter sleep mode");
        Serial.println("\nStorage:");
        Serial.println("  storage info           - Storage information");
        Serial.println("  storage ls             - List SD card contents");
        Serial.println("\nBluetooth:");
        Serial.println("  ble start              - Start BLE advertising");
        Serial.println("  ble stop               - Stop BLE advertising");
        Serial.println("  ble info               - BLE information");
        Serial.println("\nConfiguration:");
        Serial.println("  config show            - Show configuration");
        Serial.println("  config save            - Save configuration");
        Serial.println("  config reset           - Reset to defaults");
        Serial.println("===========================\n");
        return;
    }

    if (line == "sysinfo") {
        cmd_sysinfo();
        return;
    }

    if (line == "wifi scan") {
        cmd_wifi_scan();
        return;
    }

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

    if (line == "task list") {
        cmd_task_list();
        return;
    }

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

    if (line == "storage info") {
        cmd_storage_info();
        return;
    }

    if (line == "storage ls") {
        cmd_storage_ls();
        return;
    }

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

    Serial.println("Unknown command. Type 'help' for available commands.");
}