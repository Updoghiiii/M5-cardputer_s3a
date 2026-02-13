#pragma once
#include <Arduino.h>

void cmd_power_info();
void cmd_power_battery();
void cmd_power_mode();
void cmd_power_mode_set(const String& mode);
void cmd_power_sleep();