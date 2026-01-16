#include "cmd_sys.h"
#include <Arduino.h>
#include <esp_system.h>

void cmd_sysinfo() {
    Serial.println("System Info:");
    Serial.printf("  Free heap: %u\n", esp_get_free_heap_size());
}