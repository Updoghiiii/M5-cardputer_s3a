#pragma once
#include <Arduino.h>
#include <FS.h>
#include <SD.h>

class StorageManager {
public:
    static StorageManager& getInstance();

    void init();

    bool isSDCardMounted();
    bool fileExists(const char* path);
    bool createFile(const char* path);
    bool deleteFile(const char* path);
    bool writeFile(const char* path, const String& data);
    String readFile(const char* path);

    bool createDir(const char* path);
    bool deleteDir(const char* path);

    void listDir(const char* path, uint8_t levels = 0);
    void printStorageInfo();

private:
    StorageManager();

    bool sdMounted;
    uint32_t lastInitAttempt;
    uint32_t initRetryInterval;
};