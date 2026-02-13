#include "power_manager.h"
#include <M5Cardputer.h>

#define BATTERY_CHECK_INTERVAL 5000      // 5 seconds
#define DEFAULT_SLEEP_TIMEOUT 60000      // 60 seconds

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
    Serial.println("Power Manager initialized");
}

void PowerManager::update() {
    if (!batteryMonitoringEnabled) return;
    updateBatteryStatus();
    checkSleepCondition();
}

float PowerManager::getBatteryVoltage() {
    return batteryVoltage;
}

float PowerManager::getBatteryPercentage() {
    return batteryPercentage;
}

void PowerManager::setPowerMode(PowerMode_t mode) {
    if (currentMode == mode) return;

    currentMode = mode;
    Serial.printf("Power mode changed to: %s\n", getPowerModeString());

    switch (mode) {
        case POWER_MODE_NORMAL:
            M5Cardputer.Display.setTextColor(WHITE, BLACK);
            break;

        case POWER_MODE_EFFICIENCY:
            // hook for dimming / reduced refresh
            break;

        case POWER_MODE_LOW_POWER:
            // hook for aggressive dimming / shutdown
            break;

        case POWER_MODE_SLEEP:
            Serial.println("Entering sleep mode...");
            enterSleep();
            break;
    }
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
        case POWER_MODE_NORMAL:      return "Normal";
        case POWER_MODE_EFFICIENCY:  return "Efficiency";
        case POWER_MODE_LOW_POWER:   return "Low Power";
        case POWER_MODE_SLEEP:       return "Sleep";
        default:                     return "Unknown";
    }
}

#include "console.h"
void PowerManager::printBatteryInfo() {
    console_printf("Battery: %.2fV (%.1f%%) - Mode: %s",
                  batteryVoltage, batteryPercentage, getPowerModeString());
}

void PowerManager::updateBatteryStatus() {
    uint32_t now = millis();
    if (now - lastBatteryCheck < BATTERY_CHECK_INTERVAL) return;

    lastBatteryCheck = now;

    batteryVoltage = M5Cardputer.Power.getBatteryVoltage() / 1000.0f;
    batteryPercentage = M5Cardputer.Power.getBatteryLevel();

    if (batteryPercentage < 20.0f &&
        currentMode != POWER_MODE_LOW_POWER &&
        currentMode != POWER_MODE_SLEEP) {
        setPowerMode(POWER_MODE_LOW_POWER);
    }
}

void PowerManager::checkSleepCondition() {
    uint32_t idleTime = millis() - lastActivityTime;

    if (M5Cardputer.Keyboard.isPressed() > 0) {
        resetIdleTimer();
        return;
    }

    if (idleTime > sleepTimeout &&
        currentMode != POWER_MODE_SLEEP) {
        setPowerMode(POWER_MODE_SLEEP);
    }
}

void PowerManager::enterSleep() {
    Serial.println("System sleeping...");

    M5Cardputer.Display.fillScreen(BLACK);

    esp_sleep_enable_timer_wakeup(5 * 1000000ULL);
    esp_light_sleep_start();

    Serial.println("Woke from sleep");
    resetIdleTimer();
    setPowerMode(POWER_MODE_NORMAL);
}