#pragma once
#include <Arduino.h>

typedef enum {
    POWER_MODE_NORMAL = 0,
    POWER_MODE_EFFICIENCY = 1,
    POWER_MODE_LOW_POWER = 2,
    POWER_MODE_SLEEP = 3
} PowerMode_t;

class PowerManager {
public:
    static PowerManager& getInstance();
    
    void init();
    void update();
    
    float getBatteryVoltage();
    float getBatteryPercentage();
    void setPowerMode(PowerMode_t mode);
    PowerMode_t getPowerMode();
    
    void enableBatteryMonitoring(bool enable);
    void setSleepTimeout(uint32_t milliseconds);
    void resetIdleTimer();
    
    const char* getPowerModeString();
    void printBatteryInfo();
    
private:
    PowerManager();
    PowerMode_t currentMode;
    float batteryVoltage;
    float batteryPercentage;
    uint32_t lastBatteryCheck;
    uint32_t sleepTimeout;
    uint32_t lastActivityTime;
    bool batteryMonitoringEnabled;
    
    void updateBatteryStatus();
    void checkSleepCondition();
};
