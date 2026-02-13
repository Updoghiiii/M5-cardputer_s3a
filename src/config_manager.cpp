#include "config_manager.h"
#include "storage_manager.h"

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager() {
    loadDefaults();
}

void ConfigManager::init() {
    loadConfig();
    Serial.println("Config Manager initialized");
}

void ConfigManager::loadDefaults() {
    config.deviceName = "M5 CardPuter";
    config.wifiSSID = "";
    config.wifiPassword = "";
    config.bluetoothEnabled = true;
    config.displayBrightness = 200;
    config.sleepTimeout = 60000;
    config.batteryMonitoringEnabled = true;
}

void ConfigManager::loadConfig() {
    StorageManager& storage = StorageManager::getInstance();

    if (!storage.isSDCardMounted()) {
        Serial.println("SD Card not mounted, using default config");
        loadDefaults();
        return;
    }

    if (!storage.fileExists(CONFIG_FILE)) {
        Serial.println("Config file not found, creating with defaults");
        saveConfig();
        return;
    }

    String jsonString = storage.readFile(CONFIG_FILE);
    if (jsonString.length() == 0) {
        Serial.println("Config file empty, loading defaults");
        loadDefaults();
        return;
    }

    StaticJsonDocument<CONFIG_BUFFER_SIZE> doc;
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error) {
        Serial.printf("Failed to parse config: %s\n", error.c_str());
        loadDefaults();
        return;
    }

    config.deviceName = doc["deviceName"] | "M5 CardPuter";
    config.wifiSSID = doc["wifiSSID"] | "";
    config.wifiPassword = doc["wifiPassword"] | "";
    config.bluetoothEnabled = doc["bluetoothEnabled"] | true;
    config.displayBrightness = doc["displayBrightness"] | 200;
    config.sleepTimeout = doc["sleepTimeout"] | 60000;
    config.batteryMonitoringEnabled = doc["batteryMonitoringEnabled"] | true;

    Serial.println("Config loaded successfully");
}

void ConfigManager::saveConfig() {
    StorageManager& storage = StorageManager::getInstance();

    if (!storage.isSDCardMounted()) {
        Serial.println("SD Card not mounted, cannot save config");
        return;
    }

    StaticJsonDocument<CONFIG_BUFFER_SIZE> doc;

    doc["deviceName"] = config.deviceName;
    doc["wifiSSID"] = config.wifiSSID;
    doc["wifiPassword"] = config.wifiPassword;
    doc["bluetoothEnabled"] = config.bluetoothEnabled;
    doc["displayBrightness"] = config.displayBrightness;
    doc["sleepTimeout"] = config.sleepTimeout;
    doc["batteryMonitoringEnabled"] = config.batteryMonitoringEnabled;

    String jsonString;
    serializeJson(doc, jsonString);

    if (storage.writeFile(CONFIG_FILE, jsonString)) {
        Serial.println("Config saved successfully");
    } else {
        Serial.println("Failed to save config");
    }
}

void ConfigManager::resetToDefaults() {
    loadDefaults();
    saveConfig();
    Serial.println("Config reset to defaults");
}

String ConfigManager::getDeviceName() {
    return config.deviceName;
}

void ConfigManager::setDeviceName(const String& name) {
    config.deviceName = name;
}

String ConfigManager::getWifiSSID() {
    return config.wifiSSID;
}

void ConfigManager::setWifiSSID(const String& ssid) {
    config.wifiSSID = ssid;
}

String ConfigManager::getWifiPassword() {
    return config.wifiPassword;
}

void ConfigManager::setWifiPassword(const String& password) {
    config.wifiPassword = password;
}

bool ConfigManager::isBluetoothEnabled() {
    return config.bluetoothEnabled;
}

void ConfigManager::setBluetoothEnabled(bool enabled) {
    config.bluetoothEnabled = enabled;
}

int ConfigManager::getDisplayBrightness() {
    return config.displayBrightness;
}

void ConfigManager::setDisplayBrightness(int brightness) {
    config.displayBrightness = constrain(brightness, 0, 255);
}

int ConfigManager::getSleepTimeout() {
    return config.sleepTimeout;
}

void ConfigManager::setSleepTimeout(int timeout) {
    config.sleepTimeout = timeout;
}

bool ConfigManager::isBatteryMonitoringEnabled() {
    return config.batteryMonitoringEnabled;
}

void ConfigManager::setBatteryMonitoringEnabled(bool enabled) {
    config.batteryMonitoringEnabled = enabled;
}

void ConfigManager::printConfig() {
    Serial.println("=== Current Configuration ===");
    Serial.printf("Device Name: %s\n", config.deviceName.c_str());
    Serial.printf("WiFi SSID: %s\n", config.wifiSSID.c_str());
    Serial.printf("Bluetooth: %s\n", config.bluetoothEnabled ? "Enabled" : "Disabled");
    Serial.printf("Display Brightness: %d\n", config.displayBrightness);
    Serial.printf("Sleep Timeout: %d ms\n", config.sleepTimeout);
    Serial.printf("Battery Monitoring: %s\n", config.batteryMonitoringEnabled ? "Enabled" : "Disabled");
    Serial.println("============================");
}