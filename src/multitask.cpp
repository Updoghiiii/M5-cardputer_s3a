#include "multitask.h"
#include "console.h"
#include <Arduino.h>

QueueHandle_t eventQueue = nullptr;

MultitaskManager& MultitaskManager::getInstance() {
    static MultitaskManager instance;
    return instance;
}

MultitaskManager::MultitaskManager() : taskCount(0) {
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].handle = nullptr;
        tasks[i].enabled = false;
    }
}

void MultitaskManager::init() {
    if (eventQueue == nullptr) {
        eventQueue = xQueueCreate(32, sizeof(uint32_t));
        if (eventQueue == nullptr) {
            console_println("Failed to create event queue");
        }
    }
    console_println("Multitask Manager initialized");
}

TaskHandle_t MultitaskManager::createTask(const char* name, TaskFunction_t function,
                                          void* params, UBaseType_t priority, TaskType_t type) {
    if (taskCount >= MAX_TASKS) {
        console_println("Max tasks reached");
        return nullptr;
    }
    
    TaskHandle_t handle = nullptr;
    BaseType_t result = xTaskCreate(function, name, TASK_STACK_SIZE, params, priority, &handle);
    
    if (result == pdPASS && handle != nullptr) {
        tasks[taskCount].handle = handle;
        tasks[taskCount].name = name;
        tasks[taskCount].type = type;
        tasks[taskCount].priority = priority;
        tasks[taskCount].enabled = true;
        taskCount++;
        console_printf("Task created: %s (Priority: %d)", name, (int)priority);
        return handle;
    } else {
        console_printf("Failed to create task: %s", name);
        return nullptr;
    }
}

void MultitaskManager::deleteTask(TaskHandle_t handle) {
    if (handle != nullptr) {
        vTaskDelete(handle);
        for (int i = 0; i < taskCount; i++) {
            if (tasks[i].handle == handle) {
                tasks[i].enabled = false;
                break;
            }
        }
    }
}

void MultitaskManager::suspendTask(TaskHandle_t handle) {
    if (handle != nullptr) {
        vTaskSuspend(handle);
    }
}

void MultitaskManager::resumeTask(TaskHandle_t handle) {
    if (handle != nullptr) {
        vTaskResume(handle);
    }
}

void MultitaskManager::listTasks() {
    console_println("Active Tasks:");
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].enabled) {
            console_printf("  - %s (Priority: %d, Type: %d)",
                         tasks[i].name, (int)tasks[i].priority, tasks[i].type);
        }
    }
}

TickType_t MultitaskManager::getTaskUptime(TaskHandle_t handle) {
    return xTaskGetTickCount();
}
