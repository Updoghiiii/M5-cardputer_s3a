#include "multitask.h"
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
            Serial.println("Failed to create event queue");
        }
    }
    Serial.println("Multitask Manager initialized");
}

TaskHandle_t MultitaskManager::createTask(const char* name, TaskFunction_t function,
                                          void* params, UBaseType_t priority, TaskType_t type) {
    if (taskCount >= MAX_TASKS) {
        Serial.println("Max tasks reached");
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
        Serial.printf("Task created: %s (Priority: %d)\n", name, priority);
        return handle;
    } else {
        Serial.printf("Failed to create task: %s\n", name);
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
    Serial.println("Active Tasks:");
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].enabled) {
            Serial.printf("  - %s (Priority: %d, Type: %d)\n", 
                         tasks[i].name, tasks[i].priority, tasks[i].type);
        }
    }
}

TickType_t MultitaskManager::getTaskUptime(TaskHandle_t handle) {
    return xTaskGetTickCount();
}
