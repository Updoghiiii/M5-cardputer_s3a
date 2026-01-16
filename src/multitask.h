#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#define TASK_STACK_SIZE 4096
#define MAX_TASKS 8

typedef enum {
    TASK_WIFI,
    TASK_DISPLAY,
    TASK_HID,
    TASK_POWER,
    TASK_STORAGE,
    TASK_BLE,
    TASK_CONFIG,
    TASK_USER
} TaskType_t;

typedef struct {
    TaskType_t type;
    TaskHandle_t handle;
    const char* name;
    UBaseType_t priority;
    bool enabled;
} Task_t;

class MultitaskManager {
public:
    static MultitaskManager& getInstance();
    
    void init();
    TaskHandle_t createTask(const char* name, TaskFunction_t function, 
                           void* params, UBaseType_t priority, TaskType_t type);
    void deleteTask(TaskHandle_t handle);
    void suspendTask(TaskHandle_t handle);
    void resumeTask(TaskHandle_t handle);
    
    void listTasks();
    TickType_t getTaskUptime(TaskHandle_t handle);
    
private:
    MultitaskManager();
    Task_t tasks[MAX_TASKS];
    uint8_t taskCount;
};

extern QueueHandle_t eventQueue;
