#include "cmd_config.h"
#include "../config_manager.h"
#include <Arduino.h>

void cmd_config_show() {
    ConfigManager::getInstance().printConfig();
}

void cmd_config_set(const String& key, const String& value) {
    ConfigManager& cfg = ConfigManager::getInstance();

    if (key == "devicename") {
        cfg.setDeviceName(value);
        Serial.printf("Device name set to: %s\n", value.c_str());
        return;
    }

    if (key == "brightness") {
        int b = value.toInt();
        cfg.setDisplayBrightness(b);
        Serial.printf("Brightness set to: %d\n", b);
        return;
    }

    if (key == "sleeptimeout") {
        int t = value.toInt();
        cfg.setSleepTimeout(t);
        Serial.printf("Sleep timeout set to: %d ms\n", t);
        return;
    }

    if (key == "bluetooth") {
        bool enabled = (value == "1" || value == "true" || value == "on");
        cfg.setBluetoothEnabled(enabled);
        Serial.printf("Bluetooth %s\n", enabled ? "enabled" : "disabled");
        return;
    }

    if (key == "batterymonitor") {
        bool enabled = (value == "1" || value == "true" || value == "on");
        cfg.setBatteryMonitoringEnabled(enabled);
        Serial.printf("Battery monitoring %s\n", enabled ? "enabled" : "disabled");
        return;
    }

    Serial.println("Unknown config key.");
    Serial.println("Valid keys:");
    Serial.println("  devicename");
    Serial.println("  brightness");
    Serial.println("  sleeptimeout");
    Serial.println("  bluetooth");
    Serial.println("  batterymonitor");
}

void cmd_config_set() {
    Serial.println("set command requires parameters");
    Serial.println("Usage: config set <key> <value>");
    Serial.println("Available keys:");
    Serial.println("  - devicename");
    Serial.println("  - brightness");
    Serial.println("  - sleeptimeout");
    Serial.println("  - bluetooth");
    Serial.println("  - batterymonitor");
}

void cmd_config_save() {
    ConfigManager::getInstance().saveConfig();
    Serial.println("Configuration saved");
}

void cmd_config_reset() {
    ConfigManager::getInstance().resetToDefaults();
    Serial.println("Configuration reset to defaults");
}