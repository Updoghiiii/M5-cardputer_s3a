#include "cmd_multitask.h"
#include "../multitask.h"
#include <Arduino.h>

void cmd_task_list() {
    Serial.println("=== Active Tasks ===");
    MultitaskManager::getInstance().listTasks();
    Serial.println("====================");
}

void cmd_task_create() {
    Serial.println("Task creation requires specific implementation");
    Serial.println("Use task-specific commands instead (e.g., 'ble start', 'wifi scan')");
}

void cmd_task_delete() {
    Serial.println("Task deletion is managed by the task manager");
}

void cmd_task_priority() {
    Serial.println("Task priorities (0-24):");
    Serial.println("  0-9:   Low priority");
    Serial.println("  10-15: Normal priority");
    Serial.println("  16-24: High priority");
}
