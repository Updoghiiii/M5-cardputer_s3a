#include "storage_manager.h"
#include "console.h"

StorageManager& StorageManager::getInstance() {
    static StorageManager instance;
    return instance;
}

StorageManager::StorageManager()
    : sdMounted(false),
      lastInitAttempt(0),
      initRetryInterval(2000) {
}

void StorageManager::init() {
    uint32_t now = millis();
    if (now - lastInitAttempt < initRetryInterval) return;
    lastInitAttempt = now;

    console_println("Initializing SD card...");

    // M5Cardputer SD Pins: CS=12, SCK=41, MISO=40, MOSI=14
    SPI.begin(41, 40, 14, 12);
    if (!SD.begin(12, SPI, 25000000)) {
        console_println("SD Card initialization failed");
        sdMounted = false;
        return;
    }

    sdMounted = true;
    console_println("SD Card mounted successfully");
    printStorageInfo();
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
    if (!file) return false;

    file.close();
    return true;
}

bool StorageManager::deleteFile(const char* path) {
    if (!sdMounted) return false;
    return SD.remove(path);
}

bool StorageManager::writeFile(const char* path, const String& data) {
    if (!sdMounted) return false;

    String tempPath = String(path) + ".tmp";

    File file = SD.open(tempPath.c_str(), FILE_WRITE);
    if (!file) {
        Serial.printf("Failed to open file for write: %s\n", tempPath.c_str());
        return false;
    }

    size_t written = file.print(data);
    file.close();

    if (written == 0) {
        SD.remove(tempPath.c_str());
        return false;
    }

    SD.remove(path);
    SD.rename(tempPath.c_str(), path);

    return true;
}

bool StorageManager::appendFile(const char* path, const String& data) {
    if (!sdMounted) return false;

    File file = SD.open(path, FILE_APPEND);
    if (!file) {
        console_printf("Failed to open file for append: %s", path);
        return false;
    }

    size_t written = file.print(data);
    file.close();

    return (written > 0);
}

String StorageManager::readFile(const char* path) {
    if (!sdMounted) return "";

    File file = SD.open(path, FILE_READ);
    if (!file) {
        Serial.printf("Failed to open file: %s\n", path);
        return "";
    }

    String content;
    content.reserve(file.size());

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
        console_println("SD Card not mounted");
        return;
    }

    File root = SD.open(path);
    if (!root || !root.isDirectory()) {
        console_printf("Failed to open directory: %s", path);
        return;
    }

    File file = root.openNextFile();
    while (file) {
        String indent = "";
        for (uint8_t i = 0; i < levels; i++) indent += "  ";

        if (file.isDirectory()) {
            console_printf("%sDIR: %s", indent.c_str(), file.name());
            if (levels > 0) {
                listDir(file.path(), levels - 1);
            }
        } else {
            console_printf("%sFILE: %s (%llu bytes)", indent.c_str(), file.name(), file.size());
        }

        file = root.openNextFile();
    }
}

void StorageManager::printStorageInfo() {
    if (!sdMounted) {
        console_println("SD Card not mounted");
        return;
    }

    uint64_t cardSize = SD.cardSize();
    uint64_t usedSize = SD.usedBytes();

    console_printf("SD Card Size: %llu MB", cardSize / (1024 * 1024));
    console_printf("Used Space: %llu MB", usedSize / (1024 * 1024));
    console_printf("Free Space: %llu MB", (cardSize - usedSize) / (1024 * 1024));
}