#include "cmd_multitask.h"
#include "../multitask.h"
#include "../console.h"
#include <Arduino.h>

void cmd_task_list() {
    console_println("=== Active Tasks ===");
    MultitaskManager::getInstance().listTasks();
    console_println("====================");
}

void cmd_task_create() {
    console_println("Task creation requires specific implementation");
    console_println("Use task-specific commands instead (e.g., 'ble start', 'wifi scan')");
}

void cmd_task_delete() {
    console_println("Task deletion is managed by the task manager");
}

void cmd_task_priority() {
    console_println("Task priorities (0-24):");
    console_println("  0-9:   Low priority");
    console_println("  10-15: Normal priority");
    console_println("  16-24: High priority");
}
