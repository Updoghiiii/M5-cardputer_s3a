#include "cmd_storage.h"
#include "../storage_manager.h"
#include "../console.h"
#include <Arduino.h>

void cmd_storage_info() {
    console_println("=== Storage Information ===");
    StorageManager::getInstance().printStorageInfo();
    console_println("============================");
}

void cmd_storage_ls(const String& path) {
    console_printf("=== SD Card Contents: %s ===", path.c_str());
    StorageManager::getInstance().listDir(path.c_str());
    console_println("==============================");
}

void cmd_storage_ls() {
    cmd_storage_ls("/");
}

void cmd_storage_cat(const String& filename) {
    StorageManager& sm = StorageManager::getInstance();

    if (!sm.isSDCardMounted()) {
        console_println("SD Card not mounted");
        return;
    }

    if (!sm.fileExists(filename.c_str())) {
        console_printf("File not found: %s", filename.c_str());
        return;
    }

    console_printf("=== %s ===", filename.c_str());
    String content = sm.readFile(filename.c_str());
    console_println(content);
    console_println("=====================");
}

void cmd_storage_cat() {
    console_println("cat command requires filename parameter");
    console_println("Usage: storage cat <filename>");
}

void cmd_storage_rm(const String& filename) {
    StorageManager& sm = StorageManager::getInstance();

    if (!sm.isSDCardMounted()) {
        console_println("SD Card not mounted");
        return;
    }

    if (!sm.fileExists(filename.c_str())) {
        console_printf("File not found: %s", filename.c_str());
        return;
    }

    if (sm.deleteFile(filename.c_str())) {
        console_printf("Deleted: %s", filename.c_str());
    } else {
        console_printf("Failed to delete: %s", filename.c_str());
    }
}

void cmd_storage_rm() {
    console_println("rm command requires filename parameter");
    console_println("Usage: storage rm <filename>");
}

void cmd_storage_write(const String& filename, const String& text) {
    StorageManager& sm = StorageManager::getInstance();

    if (!sm.isSDCardMounted()) {
        console_println("SD Card not mounted");
        return;
    }

    if (sm.writeFile(filename.c_str(), text)) {
        console_printf("Wrote to %s", filename.c_str());
    } else {
        console_printf("Failed to write to %s", filename.c_str());
    }
}

void cmd_storage_mkdir(const String& dirname) {
    StorageManager& sm = StorageManager::getInstance();

    if (!sm.isSDCardMounted()) {
        console_println("SD Card not mounted");
        return;
    }

    if (sm.createDir(dirname.c_str())) {
        console_printf("Directory created: %s", dirname.c_str());
    } else {
        console_printf("Failed to create directory: %s", dirname.c_str());
    }
}

void cmd_storage_rmdir(const String& dirname) {
    StorageManager& sm = StorageManager::getInstance();

    if (!sm.isSDCardMounted()) {
        console_println("SD Card not mounted");
        return;
    }

    if (sm.deleteDir(dirname.c_str())) {
        console_printf("Directory removed: %s", dirname.c_str());
    } else {
        console_printf("Failed to remove directory: %s", dirname.c_str());
    }
}