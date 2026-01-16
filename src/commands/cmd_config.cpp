#include "cmd_config.h"
#include "../config_manager.h"
#include <Arduino.h>

void cmd_config_show() {
    ConfigManager::getInstance().printConfig();
}

void cmd_config_set() {
    Serial.println("set command requires parameters");
    Serial.println("Usage: config set <key> <value>");
    Serial.println("Available keys:");
    Serial.println("  - devicename");
    Serial.println("  - brightness");
    Serial.println("  - sleeptimeout");
}

void cmd_config_save() {
    ConfigManager::getInstance().saveConfig();
    Serial.println("Configuration saved");
}

void cmd_config_reset() {
    ConfigManager::getInstance().resetToDefaults();
    Serial.println("Configuration reset to defaults");
}
