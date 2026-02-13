#include "ble_manager.h"

// ------------------------------------------------------
// INTERNAL CALLBACKS
// ------------------------------------------------------
class BLEServerCallbacksImpl : public BLEServerCallbacks {
public:
    BLEServerCallbacksImpl(BLEManager* mgr) : manager(mgr) {}

    void onConnect(BLEServer* server) override {
        String addr = "unknown";
        manager->onConnected(addr);
        Serial.println("[BLE] Device connected");
    }

    void onDisconnect(BLEServer* server) override {
        manager->onDisconnected();
        Serial.println("[BLE] Device disconnected");
        BLEDevice::getAdvertising()->start();
    }

private:
    BLEManager* manager;
};

// ------------------------------------------------------
// SINGLETON
// ------------------------------------------------------
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
    connectionStatus.connectedDeviceAddress = "";
}

// ------------------------------------------------------
// INIT / DEINIT
// ------------------------------------------------------
void BLEManager::init() {
    if (initialized) return;

    Serial.println("[BLE] Initializing...");

    BLEDevice::init(BLE_DEVICE_NAME);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new BLEServerCallbacksImpl(this));

    BLEService* pService = pServer->createService("180A");

    pCharacteristic = pService->createCharacteristic(
        "2A29",
        NIMBLE_PROPERTY::READ |
        NIMBLE_PROPERTY::WRITE |
        NIMBLE_PROPERTY::NOTIFY
    );

    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setValue("Cardputer Ready");

    pService->start();

    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID("180A");
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMaxPreferred(0x12);

    pAdvertising->start();
    advertising = true;

    initialized = true;
    Serial.println("[BLE] Manager initialized");
}

void BLEManager::deinit() {
    if (!initialized) return;

    stopAdvertising();
    BLEDevice::deinit();

    initialized = false;
    pServer = nullptr;
    pCharacteristic = nullptr;

    Serial.println("[BLE] Deinitialized");
}

// ------------------------------------------------------
// STATUS
// ------------------------------------------------------
bool BLEManager::isInitialized() {
    return initialized;
}

bool BLEManager::isConnected() {
    return connectionStatus.connected;
}

bool BLEManager::isAdvertising() {
    return advertising;
}

BLEConnectionStatus_t BLEManager::getConnectionStatus() {
    return connectionStatus;
}

// ------------------------------------------------------
// ADVERTISING CONTROL
// ------------------------------------------------------
void BLEManager::startAdvertising() {
    if (!initialized || advertising) return;

    BLEDevice::getAdvertising()->start();
    advertising = true;

    Serial.println("[BLE] Advertising started");
}

void BLEManager::stopAdvertising() {
    if (!initialized || !advertising) return;

    BLEDevice::getAdvertising()->stop();
    advertising = false;

    Serial.println("[BLE] Advertising stopped");
}

// ------------------------------------------------------
// DATA TX/RX
// ------------------------------------------------------
void BLEManager::sendData(const String& data) {
    if (!pCharacteristic || !connectionStatus.connected) return;

    pCharacteristic->setValue(data.c_str());
    pCharacteristic->notify();

    Serial.printf("[BLE] Sent: %s\n", data.c_str());
}

String BLEManager::receiveData() {
    if (!pCharacteristic) return "";
    return String(pCharacteristic->getValue().c_str());
}

// ------------------------------------------------------
// INTERNAL STATUS UPDATERS
// ------------------------------------------------------
void BLEManager::onConnected(const String& addr) {
    connectionStatus.connected = true;
    connectionStatus.connectedTime = millis();
    connectionStatus.connectedDeviceAddress = addr;
}

void BLEManager::onDisconnected() {
    connectionStatus.connected = false;
    connectionStatus.connectedDeviceAddress = "";
}