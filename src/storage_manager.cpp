#include "storage_manager.h"

StorageManager& StorageManager::getInstance() {
    static StorageManager instance;
    return instance;
}

StorageManager::StorageManager() : sdMounted(false) {
}

void StorageManager::init() {
    // Try to initialize SD card
    if (!SD.begin()) {
        Serial.println("SD Card initialization failed");
        sdMounted = false;
    } else {
        Serial.println("SD Card mounted successfully");
        sdMounted = true;
        printStorageInfo();
    }
}

bool StorageManager::isSDCardMounted() {
    return sdMounted;
}

bool StorageManager::fileExists(const char* path) {
    if (!sdMounted) return false;
    return SD.exists(path);
}

bool StorageManager::createFile(const char* path) {
    if (!sdMounted) return false;
    
    File file = SD.open(path, FILE_WRITE);
    if (file) {
        file.close();
        return true;
    }
    return false;
}

bool StorageManager::deleteFile(const char* path) {
    if (!sdMounted) return false;
    return SD.remove(path);
}

bool StorageManager::writeFile(const char* path, const String& data) {
    if (!sdMounted) return false;
    
    File file = SD.open(path, FILE_WRITE);
    if (!file) {
        Serial.printf("Failed to open file: %s\n", path);
        return false;
    }
    
    size_t written = file.print(data);
    file.close();
    
    return written > 0;
}

String StorageManager::readFile(const char* path) {
    if (!sdMounted) return "";
    
    File file = SD.open(path, FILE_READ);
    if (!file) {
        Serial.printf("Failed to open file: %s\n", path);
        return "";
    }
    
    String content = "";
    while (file.available()) {
        content += (char)file.read();
    }
    file.close();
    
    return content;
}

bool StorageManager::createDir(const char* path) {
    if (!sdMounted) return false;
    return SD.mkdir(path);
}

bool StorageManager::deleteDir(const char* path) {
    if (!sdMounted) return false;
    return SD.rmdir(path);
}

void StorageManager::listDir(const char* path, uint8_t levels) {
    if (!sdMounted) {
        Serial.println("SD Card not mounted");
        return;
    }
    
    File root = SD.open(path);
    if (!root || !root.isDirectory()) {
        Serial.println("Failed to open directory");
        return;
    }
    
    File file = root.openNextFile();
    while (file) {
        for (uint8_t i = 0; i < levels; i++) {
            Serial.print("  ");
        }
        
        if (file.isDirectory()) {
            Serial.print("DIR: ");
            Serial.println(file.name());
            if (levels > 0) {
                listDir(file.path(), levels - 1);
            }
        } else {
            Serial.print("FILE: ");
            Serial.print(file.name());
            Serial.print(" (");
            Serial.print(file.size());
            Serial.println(" bytes)");
        }
        
        file = root.openNextFile();
    }
}

void StorageManager::printStorageInfo() {
    if (!sdMounted) {
        Serial.println("SD Card not mounted");
        return;
    }
    
    uint64_t cardSize = SD.cardSize();
    uint64_t usedSize = SD.usedBytes();
    
    Serial.printf("SD Card Size: %llu MB\n", cardSize / (1024 * 1024));
    Serial.printf("Used Space: %llu MB\n", usedSize / (1024 * 1024));
    Serial.printf("Free Space: %llu MB\n", (cardSize - usedSize) / (1024 * 1024));
}
