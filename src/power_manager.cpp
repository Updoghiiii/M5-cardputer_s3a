#include "power_manager.h"
#include <esp_adc_cal.h>
#include <driver/adc.h>

#define BATTERY_ADC_PIN 34
#define BATTERY_CHECK_INTERVAL 5000  // 5 seconds
#define DEFAULT_SLEEP_TIMEOUT 60000  // 60 seconds

PowerManager& PowerManager::getInstance() {
    static PowerManager instance;
    return instance;
}

PowerManager::PowerManager() 
    : currentMode(POWER_MODE_NORMAL), 
      batteryVoltage(0.0f),
      batteryPercentage(100.0f),
      lastBatteryCheck(0),
      sleepTimeout(DEFAULT_SLEEP_TIMEOUT),
      lastActivityTime(millis()),
      batteryMonitoringEnabled(true) {
}

void PowerManager::init() {
    // Initialize ADC for battery monitoring
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    
    Serial.println("Power Manager initialized");
}

void PowerManager::update() {
    if (batteryMonitoringEnabled) {
        updateBatteryStatus();
        checkSleepCondition();
    }
}

float PowerManager::getBatteryVoltage() {
    return batteryVoltage;
}

float PowerManager::getBatteryPercentage() {
    return batteryPercentage;
}

void PowerManager::setPowerMode(PowerMode_t mode) {
    currentMode = mode;
    Serial.printf("Power mode changed to: %s\n", getPowerModeString());
}

PowerMode_t PowerManager::getPowerMode() {
    return currentMode;
}

void PowerManager::enableBatteryMonitoring(bool enable) {
    batteryMonitoringEnabled = enable;
}

void PowerManager::setSleepTimeout(uint32_t milliseconds) {
    sleepTimeout = milliseconds;
}

void PowerManager::resetIdleTimer() {
    lastActivityTime = millis();
}

const char* PowerManager::getPowerModeString() {
    switch (currentMode) {
        case POWER_MODE_NORMAL:
            return "Normal";
        case POWER_MODE_EFFICIENCY:
            return "Efficiency";
        case POWER_MODE_LOW_POWER:
            return "Low Power";
        case POWER_MODE_SLEEP:
            return "Sleep";
        default:
            return "Unknown";
    }
}

void PowerManager::printBatteryInfo() {
    Serial.printf("Battery: %.2fV (%.1f%%) - Mode: %s\n", 
                  batteryVoltage, batteryPercentage, getPowerModeString());
}

void PowerManager::updateBatteryStatus() {
    uint32_t now = millis();
    if (now - lastBatteryCheck < BATTERY_CHECK_INTERVAL) {
        return;
    }
    
    lastBatteryCheck = now;
    
    // Read ADC value
    uint32_t adcValue = adc1_get_raw(ADC1_CHANNEL_6);
    
    // Convert to voltage (M5 Stack specific: multiply by 2 for voltage divider)
    batteryVoltage = (adcValue / 4095.0f) * 3.3f * 2.0f;
    
    // Estimate battery percentage (3.0V = 0%, 4.2V = 100%)
    if (batteryVoltage <= 3.0f) {
        batteryPercentage = 0.0f;
    } else if (batteryVoltage >= 4.2f) {
        batteryPercentage = 100.0f;
    } else {
        batteryPercentage = ((batteryVoltage - 3.0f) / 1.2f) * 100.0f;
    }
    
    // Auto switch to low power mode at 20%
    if (batteryPercentage < 20.0f && currentMode != POWER_MODE_LOW_POWER) {
        setPowerMode(POWER_MODE_LOW_POWER);
    }
}

void PowerManager::checkSleepCondition() {
    uint32_t idleTime = millis() - lastActivityTime;
    if (idleTime > sleepTimeout && currentMode != POWER_MODE_SLEEP) {
        setPowerMode(POWER_MODE_SLEEP);
    }
}
