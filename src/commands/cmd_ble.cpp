#include "cmd_ble.h"
#include "../ble_manager.h"
#include <Arduino.h>

void cmd_ble_start() {
    BLEManager& ble = BLEManager::getInstance();
    if (!ble.isInitialized()) {
        ble.init();
    }
    ble.startAdvertising();
    Serial.println("BLE advertising started");
}

void cmd_ble_stop() {
    BLEManager& ble = BLEManager::getInstance();
    ble.stopAdvertising();
    Serial.println("BLE advertising stopped");
}

void cmd_ble_info() {
    Serial.println("=== BLE Information ===");
    BLEManager& ble = BLEManager::getInstance();
    Serial.printf("Initialized: %s\n", ble.isInitialized() ? "Yes" : "No");
    Serial.printf("Connected: %s\n", ble.isConnected() ? "Yes" : "No");
    BLEConnectionStatus_t status = ble.getConnectionStatus();
    if (status.connected) {
        Serial.printf("Device: %s\n", status.connectedDeviceAddress.c_str());
    }
    Serial.println("=======================");
}

void cmd_ble_send() {
    Serial.println("send command requires data parameter");
    Serial.println("Usage: ble send <message>");
}
