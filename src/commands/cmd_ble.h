#pragma once
#include <Arduino.h>

void cmd_ble_start();
void cmd_ble_stop();
void cmd_ble_scan();
void cmd_ble_info();
void cmd_ble_send();
void cmd_ble_send(const String& message);