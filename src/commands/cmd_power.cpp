#include "cmd_power.h"
#include "../power_manager.h"
#include <Arduino.h>

void cmd_power_info() {
    PowerManager::getInstance().printBatteryInfo();
}

void cmd_power_battery() {
    Serial.println("=== Battery Information ===");
    PowerManager& pm = PowerManager::getInstance();
    Serial.printf("Voltage: %.2f V\n", pm.getBatteryVoltage());
    Serial.printf("Percentage: %.1f %%\n", pm.getBatteryPercentage());
    Serial.println("============================");
}

void cmd_power_mode() {
    Serial.println("=== Power Modes ===");
    Serial.println("normal      - Full performance");
    Serial.println("efficiency  - Reduced power consumption");
    Serial.println("lowpower    - Minimal power consumption");
    Serial.println("sleep       - Enter sleep mode");
    Serial.printf("Current: %s\n", PowerManager::getInstance().getPowerModeString());
    Serial.println("===================");
}

void cmd_power_mode_set(const String& mode) {
    PowerManager& pm = PowerManager::getInstance();

    if (mode == "normal") {
        pm.setPowerMode(POWER_MODE_NORMAL);
        Serial.println("Power mode set to NORMAL");
        return;
    }

    if (mode == "efficiency") {
        pm.setPowerMode(POWER_MODE_EFFICIENCY);
        Serial.println("Power mode set to EFFICIENCY");
        return;
    }

    if (mode == "lowpower") {
        pm.setPowerMode(POWER_MODE_LOW_POWER);
        Serial.println("Power mode set to LOW POWER");
        return;
    }

    if (mode == "sleep") {
        Serial.println("Initiating sleep mode...");
        pm.setPowerMode(POWER_MODE_SLEEP);
        Serial.println("Device entering sleep. Press any key to wake.");
        return;
    }

    Serial.println("Unknown mode. Valid modes:");
    Serial.println("  normal");
    Serial.println("  efficiency");
    Serial.println("  lowpower");
    Serial.println("  sleep");
}

void cmd_power_sleep() {
    Serial.println("Initiating sleep mode...");
    PowerManager::getInstance().setPowerMode(POWER_MODE_SLEEP);
    Serial.println("Device entering sleep. Press any key to wake.");
}