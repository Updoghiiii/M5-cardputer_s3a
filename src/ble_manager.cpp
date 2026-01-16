#include "ble_manager.h"

BLEManager& BLEManager::getInstance() {
    static BLEManager instance;
    return instance;
}

BLEManager::BLEManager() 
    : initialized(false), 
      advertising(false), 
      pServer(nullptr),
      pCharacteristic(nullptr) {
    connectionStatus.connected = false;
    connectionStatus.connectedTime = 0;
}

void BLEManager::init() {
    BLEDevice::init(BLE_DEVICE_NAME);
    pServer = BLEDevice::createServer();
    
    BLEService* pService = pServer->createService("180A");
    
    pCharacteristic = pService->createCharacteristic(
        "2A29",
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setValue("CardPuter Ready");
    
    pService->start();
    
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID("180A");
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);
    
    initialized = true;
    Serial.println("BLE Manager initialized");
}

void BLEManager::deinit() {
    if (initialized) {
        stopAdvertising();
        BLEDevice::deinit();
        initialized = false;
    }
}

bool BLEManager::isInitialized() {
    return initialized;
}

bool BLEManager::isConnected() {
    return connectionStatus.connected;
}

BLEConnectionStatus_t BLEManager::getConnectionStatus() {
    return connectionStatus;
}

void BLEManager::startAdvertising() {
    if (initialized && !advertising) {
        BLEDevice::getAdvertising()->start();
        advertising = true;
        Serial.println("BLE Advertising started");
    }
}

void BLEManager::stopAdvertising() {
    if (initialized && advertising) {
        BLEDevice::getAdvertising()->stop();
        advertising = false;
        Serial.println("BLE Advertising stopped");
    }
}

void BLEManager::sendData(const String& data) {
    if (pCharacteristic != nullptr && connectionStatus.connected) {
        pCharacteristic->setValue(data.c_str());
        pCharacteristic->notify();
    }
}

String BLEManager::receiveData() {
    if (pCharacteristic != nullptr) {
        return String(pCharacteristic->getValue().c_str());
    }
    return "";
}

void BLEManager::listAvailableDevices() {
    Serial.println("BLE Device discovered callback not yet implemented");
}
