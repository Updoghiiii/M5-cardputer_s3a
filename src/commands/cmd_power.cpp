#include "cmd_power.h"
#include "../power_manager.h"
#include "../console.h"
#include <Arduino.h>

void cmd_power_info() {
    PowerManager::getInstance().printBatteryInfo();
}

void cmd_power_battery() {
    console_println("=== Battery Information ===");
    PowerManager& pm = PowerManager::getInstance();
    console_printf("Voltage: %.2f V", pm.getBatteryVoltage());
    console_printf("Percentage: %.1f %%", pm.getBatteryPercentage());
    console_println("============================");
}

void cmd_power_mode() {
    console_println("=== Power Modes ===");
    console_println("normal      - Full performance");
    console_println("efficiency  - Reduced power consumption");
    console_println("lowpower    - Minimal power consumption");
    console_println("sleep       - Enter sleep mode");
    console_printf("Current: %s", PowerManager::getInstance().getPowerModeString());
    console_println("===================");
}

void cmd_power_mode_set(const String& mode) {
    PowerManager& pm = PowerManager::getInstance();

    if (mode == "normal") {
        pm.setPowerMode(POWER_MODE_NORMAL);
        console_println("Power mode set to NORMAL");
        return;
    }

    if (mode == "efficiency") {
        pm.setPowerMode(POWER_MODE_EFFICIENCY);
        console_println("Power mode set to EFFICIENCY");
        return;
    }

    if (mode == "lowpower") {
        pm.setPowerMode(POWER_MODE_LOW_POWER);
        console_println("Power mode set to LOW POWER");
        return;
    }

    if (mode == "sleep") {
        console_println("Initiating sleep mode...");
        pm.setPowerMode(POWER_MODE_SLEEP);
        console_println("Device entering sleep. Press any key to wake.");
        return;
    }

    console_println("Unknown mode. Valid modes:");
    console_println("  normal");
    console_println("  efficiency");
    console_println("  lowpower");
    console_println("  sleep");
}

void cmd_power_sleep() {
    console_println("Initiating sleep mode...");
    PowerManager::getInstance().setPowerMode(POWER_MODE_SLEEP);
    console_println("Device entering sleep. Press any key to wake.");
}