#pragma once
#include <Arduino.h>

void cmd_config_show();
void cmd_config_set();                      // usage message
void cmd_config_set(const String& key, const String& value);
void cmd_config_save();
void cmd_config_reset();