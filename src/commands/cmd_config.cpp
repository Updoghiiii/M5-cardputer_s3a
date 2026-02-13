#include "cmd_config.h"
#include "../config_manager.h"
#include "../console.h"
#include <Arduino.h>

void cmd_config_show() {
    ConfigManager::getInstance().printConfig();
}

void cmd_config_set(const String& key, const String& value) {
    ConfigManager& cfg = ConfigManager::getInstance();

    if (key == "devicename") {
        cfg.setDeviceName(value);
        console_printf("Device name set to: %s", value.c_str());
        return;
    }

    if (key == "brightness") {
        int b = value.toInt();
        cfg.setDisplayBrightness(b);
        console_printf("Brightness set to: %d", b);
        return;
    }

    if (key == "sleeptimeout") {
        int t = value.toInt();
        cfg.setSleepTimeout(t);
        console_printf("Sleep timeout set to: %d ms", t);
        return;
    }

    if (key == "bluetooth") {
        bool enabled = (value == "1" || value == "true" || value == "on");
        cfg.setBluetoothEnabled(enabled);
        console_printf("Bluetooth %s", enabled ? "enabled" : "disabled");
        return;
    }

    if (key == "batterymonitor") {
        bool enabled = (value == "1" || value == "true" || value == "on");
        cfg.setBatteryMonitoringEnabled(enabled);
        console_printf("Battery monitoring %s", enabled ? "enabled" : "disabled");
        return;
    }

    console_println("Unknown config key.");
    console_println("Valid keys:");
    console_println("  devicename");
    console_println("  brightness");
    console_println("  sleeptimeout");
    console_println("  bluetooth");
    console_println("  batterymonitor");
}

void cmd_config_set() {
    console_println("set command requires parameters");
    console_println("Usage: config set <key> <value>");
    console_println("Available keys:");
    console_println("  - devicename");
    console_println("  - brightness");
    console_println("  - sleeptimeout");
    console_println("  - bluetooth");
    console_println("  - batterymonitor");
}

void cmd_config_save() {
    ConfigManager::getInstance().saveConfig();
    console_println("Configuration saved");
}

void cmd_config_reset() {
    ConfigManager::getInstance().resetToDefaults();
    console_println("Configuration reset to defaults");
}