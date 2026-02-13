#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

#define CONFIG_FILE "/config.json"
#define CONFIG_BUFFER_SIZE 1024

typedef struct {
    String deviceName;
    String wifiSSID;
    String wifiPassword;
    bool bluetoothEnabled;
    int displayBrightness;
    int sleepTimeout;
    bool batteryMonitoringEnabled;
} Config_t;

class ConfigManager {
public:
    static ConfigManager& getInstance();

    void init();

    void loadConfig();
    void saveConfig();
    void resetToDefaults();

    String getDeviceName();
    void setDeviceName(const String& name);

    String getWifiSSID();
    void setWifiSSID(const String& ssid);

    String getWifiPassword();
    void setWifiPassword(const String& password);

    bool isBluetoothEnabled();
    void setBluetoothEnabled(bool enabled);

    int getDisplayBrightness();
    void setDisplayBrightness(int brightness);

    int getSleepTimeout();
    void setSleepTimeout(int timeout);

    bool isBatteryMonitoringEnabled();
    void setBatteryMonitoringEnabled(bool enabled);

    void printConfig();

private:
    ConfigManager();

    void loadDefaults();

    Config_t config;
};