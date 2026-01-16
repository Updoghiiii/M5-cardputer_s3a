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
    Serial.println("Normal: Full performance");
    Serial.println("Efficiency: Reduced power consumption");
    Serial.println("Low Power: Minimal power consumption");
    Serial.println("Sleep: Device in sleep mode");
    Serial.printf("Current: %s\n", PowerManager::getInstance().getPowerModeString());
    Serial.println("===================");
}

void cmd_power_sleep() {
    Serial.println("Initiating sleep mode...");
    PowerManager::getInstance().setPowerMode(POWER_MODE_SLEEP);
    Serial.println("Device entering sleep. Press any key to wake.");
}
