#pragma once
#include <Arduino.h>
#include <SD.h>
#include <FS.h>

class StorageManager {
public:
    static StorageManager& getInstance();
    
    void init();
    bool isSDCardMounted();
    
    // File operations
    bool fileExists(const char* path);
    bool createFile(const char* path);
    bool deleteFile(const char* path);
    bool writeFile(const char* path, const String& data);
    String readFile(const char* path);
    
    // Directory operations
    bool createDir(const char* path);
    bool deleteDir(const char* path);
    void listDir(const char* path, uint8_t levels = 0);
    
    // Storage info
    void printStorageInfo();
    
private:
    StorageManager();
    bool sdMounted;
};
