#include "cmd_storage.h"
#include "../storage_manager.h"
#include <Arduino.h>

void cmd_storage_info() {
    Serial.println("=== Storage Information ===");
    StorageManager::getInstance().printStorageInfo();
    Serial.println("============================");
}

void cmd_storage_ls() {
    Serial.println("=== SD Card Contents ===");
    StorageManager::getInstance().listDir("/");
    Serial.println("========================");
}

void cmd_storage_cat() {
    Serial.println("cat command requires filename parameter");
    Serial.println("Usage: storage cat <filename>");
}

void cmd_storage_rm() {
    Serial.println("rm command requires filename parameter");
    Serial.println("Usage: storage rm <filename>");
}
