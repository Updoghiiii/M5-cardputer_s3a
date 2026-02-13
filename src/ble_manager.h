#pragma once
#include <NimBLEDevice.h>
#include <NimBLEServer.h>
#include <NimBLECharacteristic.h>
#include <NimBLEDescriptor.h>

#define BLE_DEVICE_NAME "Cardputer"

typedef struct {
    bool connected;
    unsigned long connectedTime;
    String connectedDeviceAddress;
} BLEConnectionStatus_t;

class BLEManager {
public:
    static BLEManager& getInstance();

    void init();
    void deinit();

    bool isInitialized();
    bool isConnected();
    bool isAdvertising();

    BLEConnectionStatus_t getConnectionStatus();

    void startAdvertising();
    void stopAdvertising();
    void scan();

    void sendData(const String& data);
    String receiveData();

    void onConnected(const String& addr);
    void onDisconnected();

private:
    BLEManager();

    bool initialized;
    bool advertising;

    BLEServer* pServer;
    BLECharacteristic* pCharacteristic;

    BLEConnectionStatus_t connectionStatus;
};