#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define BLE_DEVICE_NAME "M5 CardPuter"

typedef struct {
    bool connected;
    String connectedDeviceAddress;
    uint32_t connectedTime;
} BLEConnectionStatus_t;

class BLEManager {
public:
    static BLEManager& getInstance();
    
    void init();
    void deinit();
    
    bool isInitialized();
    bool isConnected();
    BLEConnectionStatus_t getConnectionStatus();
    
    void startAdvertising();
    void stopAdvertising();
    
    void sendData(const String& data);
    String receiveData();
    
    void listAvailableDevices();
    
private:
    BLEManager();
    bool initialized;
    bool advertising;
    BLEServer* pServer;
    BLECharacteristic* pCharacteristic;
    BLEConnectionStatus_t connectionStatus;
};
