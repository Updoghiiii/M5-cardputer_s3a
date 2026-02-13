#include "cmd_storage.h"
#include "../storage_manager.h"
#include <Arduino.h>

void cmd_storage_info() {
    Serial.println("=== Storage Information ===");
    StorageManager::getInstance().printStorageInfo();
    Serial.println("============================");
}

void cmd_storage_ls(const String& path) {
    Serial.printf("=== SD Card Contents: %s ===\n", path.c_str());
    StorageManager::getInstance().listDir(path.c_str());
    Serial.println("==============================");
}

void cmd_storage_ls() {
    cmd_storage_ls("/");
}

void cmd_storage_cat(const String& filename) {
    StorageManager& sm = StorageManager::getInstance();

    if (!sm.isSDCardMounted()) {
        Serial.println("SD Card not mounted");
        return;
    }

    if (!sm.fileExists(filename.c_str())) {
        Serial.printf("File not found: %s\n", filename.c_str());
        return;
    }

    Serial.printf("=== %s ===\n", filename.c_str());
    String content = sm.readFile(filename.c_str());
    Serial.println(content);
    Serial.println("=====================");
}

void cmd_storage_cat() {
    Serial.println("cat command requires filename parameter");
    Serial.println("Usage: storage cat <filename>");
}

void cmd_storage_rm(const String& filename) {
    StorageManager& sm = StorageManager::getInstance();

    if (!sm.isSDCardMounted()) {
        Serial.println("SD Card not mounted");
        return;
    }

    if (!sm.fileExists(filename.c_str())) {
        Serial.printf("File not found: %s\n", filename.c_str());
        return;
    }

    if (sm.deleteFile(filename.c_str())) {
        Serial.printf("Deleted: %s\n", filename.c_str());
    } else {
        Serial.printf("Failed to delete: %s\n", filename.c_str());
    }
}

void cmd_storage_rm() {
    Serial.println("rm command requires filename parameter");
    Serial.println("Usage: storage rm <filename>");
}

void cmd_storage_write(const String& filename, const String& text) {
    StorageManager& sm = StorageManager::getInstance();

    if (!sm.isSDCardMounted()) {
        Serial.println("SD Card not mounted");
        return;
    }

    if (sm.writeFile(filename.c_str(), text)) {
        Serial.printf("Wrote to %s\n", filename.c_str());
    } else {
        Serial.printf("Failed to write to %s\n", filename.c_str());
    }
}

void cmd_storage_mkdir(const String& dirname) {
    StorageManager& sm = StorageManager::getInstance();

    if (!sm.isSDCardMounted()) {
        Serial.println("SD Card not mounted");
        return;
    }

    if (sm.createDir(dirname.c_str())) {
        Serial.printf("Directory created: %s\n", dirname.c_str());
    } else {
        Serial.printf("Failed to create directory: %s\n", dirname.c_str());
    }
}

void cmd_storage_rmdir(const String& dirname) {
    StorageManager& sm = StorageManager::getInstance();

    if (!sm.isSDCardMounted()) {
        Serial.println("SD Card not mounted");
        return;
    }

    if (sm.deleteDir(dirname.c_str())) {
        Serial.printf("Directory removed: %s\n", dirname.c_str());
    } else {
        Serial.printf("Failed to remove directory: %s\n", dirname.c_str());
    }
}