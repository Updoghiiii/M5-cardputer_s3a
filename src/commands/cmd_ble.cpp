#include "cmd_ble.h"
#include "../ble_manager.h"
#include "../console.h"
#include <Arduino.h>

void cmd_ble_start() {
    BLEManager& ble = BLEManager::getInstance();

    if (!ble.isInitialized()) {
        ble.init();
    }

    ble.startAdvertising();
}

void cmd_ble_stop() {
    BLEManager& ble = BLEManager::getInstance();
    ble.stopAdvertising();
}

void cmd_ble_scan() {
    BLEManager& ble = BLEManager::getInstance();
    ble.scan();
}

void cmd_ble_info() {
    BLEManager& ble = BLEManager::getInstance();

    console_println("=== BLE Information ===");
    console_printf("Initialized : %s", ble.isInitialized() ? "Yes" : "No");
    console_printf("Advertising : %s", ble.isAdvertising() ? "Yes" : "No");
    console_printf("Connected   : %s", ble.isConnected() ? "Yes" : "No");

    BLEConnectionStatus_t status = ble.getConnectionStatus();
    if (status.connected) {
        console_printf("Device Addr : %s", status.connectedDeviceAddress.c_str());
        console_printf("Connected For: %lu ms", millis() - status.connectedTime);
    }

    console_println("========================");
}

void cmd_ble_send(const String& message) {
    BLEManager& ble = BLEManager::getInstance();

    if (!ble.isInitialized()) {
        console_println("[BLE] Not initialized");
        return;
    }

    if (!ble.isConnected()) {
        console_println("[BLE] No device connected");
        return;
    }

    ble.sendData(message);
}

void cmd_ble_send() {
    console_println("send command requires data parameter");
    console_println("Usage: ble send <message>");
}