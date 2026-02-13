#include "cmd_sys.h"
#include "../console.h"
#include <Arduino.h>
#include <esp_system.h>

void cmd_sysinfo() {
    console_println("System Info:");
    console_printf("  Free heap: %u", esp_get_free_heap_size());
}