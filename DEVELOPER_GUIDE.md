# M5 Card Puter S3A - Developer's Guide

## Overview

This guide is for developers who want to understand, extend, or modify the M5 Card Puter S3A firmware.

---

## Architecture Overview

```
┌─────────────────────────────────────────────────────────┐
│                    MAIN APPLICATION LOOP                │
│                      (main.cpp)                         │
└──────────────────────────┬──────────────────────────────┘
                           │
        ┌──────────────────┼──────────────────┐
        │                  │                  │
    ┌───▼────┐        ┌───▼────┐        ┌───▼────┐
    │Keyboard│        │Managers│        │Console │
    │  Input │        │ Updates│        │Parser  │
    └────────┘        └────────┘        └────────┘
        │                  │                  │
        └──────────────────┼──────────────────┘
                           │
        ┌──────────────────┼──────────────────┐
        │                  │                  │
    ┌───▼──────────┐  ┌───▼──────────┐  ┌───▼──────────┐
    │   Command    │  │   Managers   │  │   Hardware   │
    │  Dispatcher  │  │   (6 total)  │  │   Interface  │
    └──────────────┘  └──────────────┘  └──────────────┘
```

---

## Core Manager Classes

### 1. MultitaskManager

**Purpose:** Manage FreeRTOS tasks and event queues

**Key Methods:**
```cpp
// Get singleton instance
MultitaskManager& manager = MultitaskManager::getInstance();

// Initialize
manager.init();

// Create task
TaskHandle_t task = manager.createTask(
    "TaskName",           // Task name
    taskFunction,         // Function pointer
    nullptr,              // Parameters
    tskIDLE_PRIORITY + 1, // Priority
    TASK_USER             // Task type
);

// Task lifecycle
manager.suspendTask(task);
manager.resumeTask(task);
manager.deleteTask(task);
manager.listTasks();
```

**Key Constants:**
```cpp
#define TASK_STACK_SIZE 4096
#define MAX_TASKS 8
#define DEFAULT_QUEUE_SIZE 32
```

---

### 2. DisplayManager

**Purpose:** Manage multi-window display system

**Key Methods:**
```cpp
DisplayManager& display = DisplayManager::getInstance();

// Initialize
display.init();

// Window management
int windowId = display.createWindow(x, y, width, height, "Title");
display.writeToWindow(windowId, "Content");
display.clearWindow(windowId);
display.closeWindow(windowId);

// Rendering
display.update();
display.drawAllWindows();
display.drawBorder(x, y, width, height, color);
display.drawStatusBar("Status text");
display.setWindowVisible(windowId, true);
```

**Display Coordinates:**
```
Display Size: 240 x 135 pixels
(0,0) is top-left corner
(239, 134) is bottom-right corner
```

---

### 3. PowerManager

**Purpose:** Battery monitoring and power mode management

**Key Methods:**
```cpp
PowerManager& power = PowerManager::getInstance();

// Initialize
power.init();

// Query battery status
float voltage = power.getBatteryVoltage();
float percentage = power.getBatteryPercentage();

// Power modes
power.setPowerMode(POWER_MODE_NORMAL);
PowerMode_t mode = power.getPowerMode();
const char* modeStr = power.getPowerModeString();

// Configuration
power.enableBatteryMonitoring(true);
power.setSleepTimeout(60000);
power.resetIdleTimer();

// Updates
power.update(); // Call regularly in main loop
power.printBatteryInfo();
```

**Power Modes:**
```cpp
enum PowerMode_t {
    POWER_MODE_NORMAL = 0,      // Full performance
    POWER_MODE_EFFICIENCY = 1,  // Moderate reduction
    POWER_MODE_LOW_POWER = 2,   // Minimal power
    POWER_MODE_SLEEP = 3        // Deep sleep
};
```

---

### 4. StorageManager

**Purpose:** SD card file and directory operations

**Key Methods:**
```cpp
StorageManager& storage = StorageManager::getInstance();

// Initialize
storage.init();

// Check status
bool mounted = storage.isSDCardMounted();

// File operations
bool exists = storage.fileExists("/file.txt");
storage.createFile("/file.txt");
storage.deleteFile("/file.txt");
storage.writeFile("/file.txt", "content");
String data = storage.readFile("/file.txt");

// Directory operations
storage.createDir("/mydir");
storage.deleteDir("/mydir");
storage.listDir("/");
storage.listDir("/", 2); // 2 levels deep

// Information
storage.printStorageInfo();
```

**Configuration File:**
```
Default location: /config.json
Format: JSON (human-readable)
Max size: 2KB
Auto-created on first save
```

---

### 5. BLEManager

**Purpose:** Bluetooth Low Energy connectivity

**Key Methods:**
```cpp
BLEManager& ble = BLEManager::getInstance();

// Initialize
ble.init();

// Advertising
ble.startAdvertising();
ble.stopAdvertising();

// Connection status
bool initialized = ble.isInitialized();
bool connected = ble.isConnected();
BLEConnectionStatus_t status = ble.getConnectionStatus();

// Data transmission
ble.sendData("message");
String data = ble.receiveData();

// Cleanup
ble.deinit();
```

**BLE Configuration:**
```cpp
#define BLE_DEVICE_NAME "M5 CardPuter"
Service UUID:        "180A" (Device Information)
Characteristic UUID: "2A29"
Characteristic Properties:
  - READ
  - WRITE
  - NOTIFY
```

---

### 6. ConfigManager

**Purpose:** Persistent configuration storage

**Key Methods:**
```cpp
ConfigManager& config = ConfigManager::getInstance();

// Initialize (loads from SD if available)
config.init();

// Device settings
String name = config.getDeviceName();
config.setDeviceName("NewName");

// WiFi settings
String ssid = config.getWifiSSID();
config.setWifiSSID("MyNetwork");
String pwd = config.getWifiPassword();
config.setWifiPassword("password");

// Bluetooth
bool ble = config.isBluetoothEnabled();
config.setBluetoothEnabled(true);

// Power settings
int brightness = config.getDisplayBrightness();
config.setDisplayBrightness(200);

int timeout = config.getSleepTimeout();
config.setSleepTimeout(60000);

bool monitoring = config.isBatteryMonitoringEnabled();
config.setBatteryMonitoringEnabled(true);

// File operations
config.loadConfig();
config.saveConfig();
config.resetToDefaults();
config.printConfig();
```

**Configuration Structure:**
```json
{
  "deviceName": "M5 CardPuter",
  "wifiSSID": "",
  "wifiPassword": "",
  "bluetoothEnabled": true,
  "displayBrightness": 200,
  "sleepTimeout": 60000,
  "batteryMonitoringEnabled": true
}
```

---

## Adding New Commands

### Step 1: Create Command Header
```cpp
// src/commands/cmd_myfeature.h
#pragma once
void cmd_myfeature_start();
void cmd_myfeature_stop();
void cmd_myfeature_status();
```

### Step 2: Implement Commands
```cpp
// src/commands/cmd_myfeature.cpp
#include "cmd_myfeature.h"
#include <Arduino.h>

void cmd_myfeature_start() {
    Serial.println("Feature started");
}

void cmd_myfeature_stop() {
    Serial.println("Feature stopped");
}

void cmd_myfeature_status() {
    Serial.println("Feature status: Active");
}
```

### Step 3: Update Console
```cpp
// In src/console.cpp
#include "commands/cmd_myfeature.h"

void console_process_line(const String& line) {
    // ... existing code ...
    
    if (line == "myfeature start") {
        cmd_myfeature_start();
        return;
    }
    
    if (line == "myfeature stop") {
        cmd_myfeature_stop();
        return;
    }
    
    if (line == "myfeature status") {
        cmd_myfeature_status();
        return;
    }
    
    // ... rest of code ...
}
```

### Step 4: Update Help Text
In the `help` command section, add:
```cpp
if (line == "help") {
    Serial.println("My Feature:");
    Serial.println("  myfeature start   - Start feature");
    Serial.println("  myfeature stop    - Stop feature");
    Serial.println("  myfeature status  - Feature status");
    // ... other help text ...
}
```

---

## Creating a Custom Task

### Example: Battery Monitor Task
```cpp
// Define task function
void batteryMonitorTask(void* param) {
    while(1) {
        PowerManager& power = PowerManager::getInstance();
        power.update();
        
        if (power.getBatteryPercentage() < 15.0f) {
            Serial.println("⚠️ Low Battery!");
        }
        
        // Sleep for 10 seconds
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

// Create task in setup() or console command
MultitaskManager& manager = MultitaskManager::getInstance();
manager.createTask("BatteryMonitor", batteryMonitorTask, 
                  nullptr, tskIDLE_PRIORITY + 2, TASK_POWER);
```

---

## Memory Management Guidelines

### Stack vs Heap
```cpp
// Use stack for small, temporary data
char buffer[256];  // ✓ OK (on stack)

// Use heap for large or dynamic data
char* largeBuffer = (char*)malloc(4096);  // Heap allocation

// In FreeRTOS context
String text = "Hello";  // Arduino String (uses heap)
```

### Recommended Task Stack Sizes
```cpp
#define SMALL_STACK   2048   // Simple tasks
#define NORMAL_STACK  4096   // Most tasks
#define LARGE_STACK   8192   // Complex operations
```

---

## Serial Communication

### Standard Output
```cpp
// Print to serial (115200 baud)
Serial.print("Simple text");
Serial.println("Text with newline");
Serial.printf("Formatted: %d, %s\n", value, string);
```

### Input from Serial Monitor
Commands are processed in `console.cpp`:
```cpp
void console_process_line(const String& line);
```

### Baud Rate
All serial communication uses **115200 baud** as configured in:
- `platformio.ini`
- `main.cpp` (`Serial.begin(115200)`)

---

## Hardware Interfaces

### GPIO Usage
```cpp
// Button A
M5Cardputer.BtnA.isPressed()
M5Cardputer.BtnA.wasPressed()

// Button B
M5Cardputer.BtnB.isPressed()
M5Cardputer.BtnB.wasPressed()

// Display
M5Cardputer.Display.print("text")
M5Cardputer.Display.fillScreen(color)
M5Cardputer.Display.setTextSize(size)

// Keyboard
M5Cardputer.Keyboard.isChange()
M5Cardputer.Keyboard.keyList()
M5Cardputer.Keyboard.getKeyValue(coord)
M5Cardputer.Keyboard.keysState()
```

### ADC (Battery Monitoring)
```cpp
adc1_config_width(ADC_WIDTH_BIT_12);
adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
uint32_t value = adc1_get_raw(ADC1_CHANNEL_6);
float voltage = (value / 4095.0f) * 3.3f * 2.0f;
```

---

## Error Handling Patterns

### Manager Initialization
```cpp
if (!storage.isSDCardMounted()) {
    Serial.println("SD Card not available");
    return;
}
```

### BLE Status
```cpp
if (!ble.isInitialized()) {
    ble.init();
}

if (ble.isConnected()) {
    ble.sendData("message");
}
```

### Power Mode Checking
```cpp
PowerMode_t mode = power.getPowerMode();
if (mode == POWER_MODE_SLEEP) {
    // Device is sleeping
}
```

---

## Building and Debugging

### Build Targets
```bash
# Clean build
platformio run --target clean

# Build only
platformio run

# Build specific environment
platformio run -e m5stack-stamps3

# Verbose output
platformio run -v
```

### Upload Options
```bash
# Upload to device
platformio run --target upload

# Upload and monitor
platformio run --target upload --target monitor

# Monitor only
platformio run --target monitor
```

### Debugging Tips
```cpp
// Add debug output
#ifdef DEBUG
    Serial.println("Debug message");
#endif

// Monitor memory usage
Serial.printf("Free heap: %u\n", esp_get_free_heap_size());

// Task information
MultitaskManager::getInstance().listTasks();

// Power status
PowerManager::getInstance().printBatteryInfo();

// Configuration
ConfigManager::getInstance().printConfig();
```

---

## Testing Checklist

Before deploying new code:

- [ ] Code compiles without errors
- [ ] No undefined references
- [ ] All includes are correct
- [ ] New commands appear in `help`
- [ ] Serial output is correct
- [ ] Power consumption is acceptable
- [ ] Memory usage is within limits
- [ ] Tasks don't interfere with each other
- [ ] Configuration persists after reboot
- [ ] All features work as expected

---

## Performance Optimization

### Memory Optimization
```cpp
// Use const where possible
const char* message = "Static string";

// Avoid dynamic allocation in loops
for (int i = 0; i < 100; i++) {
    String temp;  // ❌ Bad
}

std::vector<String> items;  // ✓ Allocate once
```

### CPU Optimization
```cpp
// Add delays to prevent watchdog timeout
void myTask(void* param) {
    while(1) {
        // Do work
        vTaskDelay(100 / portTICK_PERIOD_MS);  // Yield CPU
    }
}
```

### Power Optimization
```cpp
// Use appropriate power modes
if (idle_time > 60000) {
    power.setPowerMode(POWER_MODE_SLEEP);
}

// Reduce display updates
display.update();  // Once per loop, not multiple times
```

---

## Common Issues and Solutions

### SD Card Not Found
```cpp
if (!storage.isSDCardMounted()) {
    // Check physical connection
    // Try reformatting to FAT32
    // Verify SD card speed (Class 4+)
}
```

### BLE Connection Drops
```cpp
// Restart BLE
ble.stopAdvertising();
vTaskDelay(1000 / portTICK_PERIOD_MS);
ble.startAdvertising();
```

### High Power Consumption
```cpp
// Check active tasks
manager.listTasks();  // Too many tasks?

// Reduce brightness
config.setDisplayBrightness(100);

// Enable low power mode
power.setPowerMode(POWER_MODE_LOW_POWER);
```

### Memory Leak
```cpp
// Monitor heap
Serial.printf("Free: %u, Used: %u\n", 
              esp_get_free_heap_size(),
              heap_caps_get_total_size(MALLOC_CAP_8BIT));
```

---

## Repository Structure

```
For extending the project:
├── src/
│   ├── New feature modules
│   └── commands/
│       └── New command handlers
├── platformio.ini (Update dependencies)
├── sdkconfig.defaults (ESP-IDF config)
└── Documentation files
```

---

## Resources

### Official Documentation
- ESP32-S3: https://docs.espressif.com/
- M5Stack: https://docs.m5stack.com/
- FreeRTOS: https://www.freertos.org/

### Libraries Used
- ArduinoJson: https://github.com/bblanchon/ArduinoJson
- NimBLE: https://github.com/h2zero/NimBLE-Arduino
- M5Cardputer: https://github.com/m5stack/M5Cardputer

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 1.0.0 | Jan 2026 | Initial release - All 7 features implemented |

---

## Support

For issues or questions:
1. Check the FIRMWARE_FEATURES.md for API docs
2. Review example code in command modules
3. Check serial output for error messages
4. Review memory usage with `sysinfo`

---

**Last Updated:** January 2026
**Compatible Hardware:** M5 Stack Card Puter S3A
**Framework:** Arduino + ESP-IDF
**Language:** C++
