#include "cmd_ble.h"
#include "../ble_manager.h"
#include <Arduino.h>

void cmd_ble_start() {
    BLEManager& ble = BLEManager::getInstance();

    if (!ble.isInitialized()) {
        Serial.println("[BLE] Initializing...");
        ble.init();
    }

    ble.startAdvertising();
    Serial.println("[BLE] Advertising started");
}

void cmd_ble_stop() {
    BLEManager& ble = BLEManager::getInstance();
    ble.stopAdvertising();
    Serial.println("[BLE] Advertising stopped");
}

void cmd_ble_info() {
    BLEManager& ble = BLEManager::getInstance();

    Serial.println("=== BLE Information ===");
    Serial.printf("Initialized : %s\n", ble.isInitialized() ? "Yes" : "No");
    Serial.printf("Advertising : %s\n", ble.isAdvertising() ? "Yes" : "No");
    Serial.printf("Connected   : %s\n", ble.isConnected() ? "Yes" : "No");

    BLEConnectionStatus_t status = ble.getConnectionStatus();
    if (status.connected) {
        Serial.printf("Device Addr : %s\n", status.connectedDeviceAddress.c_str());
        Serial.printf("Connected For: %lu ms\n", millis() - status.connectedTime);
    }

    Serial.println("========================");
}

void cmd_ble_send(const String& message) {
    BLEManager& ble = BLEManager::getInstance();

    if (!ble.isInitialized()) {
        Serial.println("[BLE] Not initialized");
        return;
    }

    if (!ble.isConnected()) {
        Serial.println("[BLE] No device connected");
        return;
    }

    ble.sendData(message);
    Serial.printf("[BLE] Sent: %s\n", message.c_str());
}

void cmd_ble_send() {
    Serial.println("send command requires data parameter");
    Serial.println("Usage: ble send <message>");
}