#pragma once
#include <Arduino.h>

void cmd_storage_info();
void cmd_storage_ls();
void cmd_storage_ls(const String& path);
void cmd_storage_cat();
void cmd_storage_cat(const String& filename);
void cmd_storage_rm();
void cmd_storage_rm(const String& filename);
void cmd_storage_write(const String& filename, const String& text);
void cmd_storage_mkdir(const String& dirname);
void cmd_storage_rmdir(const String& dirname);