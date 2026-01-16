# M5 Card Puter S3A - Complete Multitasking Firmware

This is a fully-featured firmware for the M5 Card Puter S3A device with comprehensive support for multitasking, power management, storage, Bluetooth/BLE connectivity, and configuration management.

## Features Implemented

### 1. **Enhanced HID (Human Interface Device) Support**
   - **Location:** `src/commands/cmd_hid.cpp` and `src/commands/cmd_hid.h`
   - **Features:**
     - Keyboard input testing and validation
     - Button state monitoring (Button A & B)
     - Mouse emulation support
     - Gamepad functionality
   - **Commands:**
     - `hid test` - Full HID test suite
     - `hid info` - Keyboard information
     - `hid mouse` - Mouse emulation details
     - `hid gamepad` - Gamepad test

### 2. **Multitasking Framework (FreeRTOS)**
   - **Location:** `src/multitask.h` and `src/multitask.cpp`
   - **Features:**
     - FreeRTOS task management with priority scheduling
     - Support for up to 8 concurrent tasks
     - Task creation, deletion, suspension, and resumption
     - Event queue for inter-task communication
     - Task uptime tracking
   - **Commands:**
     - `task list` - List all active tasks
   - **API:**
     ```cpp
     MultitaskManager& manager = MultitaskManager::getInstance();
     manager.init();
     TaskHandle_t task = manager.createTask("TaskName", taskFunction, 
                                           nullptr, tskIDLE_PRIORITY + 1, TASK_TYPE);
     ```

### 3. **Enhanced Display Management**
   - **Location:** `src/display_manager.h` and `src/display_manager.cpp`
   - **Features:**
     - Multi-window support (up to 4 windows)
     - Window management (create, close, show/hide)
     - Text rendering with window boundaries
     - Status bar support
     - Border drawing
   - **API:**
     ```cpp
     DisplayManager& display = DisplayManager::getInstance();
     int windowId = display.createWindow(10, 40, 220, 150, "Window Title");
     display.writeToWindow(windowId, "Text content");
     ```

### 4. **Power Management System**
   - **Location:** `src/power_manager.h` and `src/power_manager.cpp`
   - **Features:**
     - Battery voltage monitoring via ADC
     - Battery percentage estimation
     - Power mode selection (Normal, Efficiency, Low Power, Sleep)
     - Idle timeout with automatic sleep mode
     - Automatic low-power mode at 20% battery
   - **Commands:**
     - `power info` - Current power status
     - `power battery` - Battery voltage and percentage
     - `power mode` - Available power modes
     - `power sleep` - Enter sleep mode
   - **Power Modes:**
     - **Normal:** Full performance
     - **Efficiency:** Reduced power consumption
     - **Low Power:** Minimal consumption
     - **Sleep:** Deep sleep with minimal power draw

### 5. **SD Card Storage Management**
   - **Location:** `src/storage_manager.h` and `src/storage_manager.cpp`
   - **Features:**
     - SD card initialization and mounting
     - File read/write operations
     - Directory listing and traversal
     - File/directory creation and deletion
     - Storage space information
   - **Commands:**
     - `storage info` - Storage space details
     - `storage ls` - List SD card contents
   - **API:**
     ```cpp
     StorageManager& storage = StorageManager::getInstance();
     if (storage.isSDCardMounted()) {
         storage.writeFile("/test.txt", "content");
         String data = storage.readFile("/test.txt");
     }
     ```

### 6. **Bluetooth/BLE Connectivity**
   - **Location:** `src/ble_manager.h` and `src/ble_manager.cpp`
   - **Features:**
     - BLE server implementation
     - Device advertising
     - Data transmission and reception
     - Connection status tracking
     - Service and characteristic management
   - **Commands:**
     - `ble start` - Start BLE advertising
     - `ble stop` - Stop advertising
     - `ble info` - BLE connection status
   - **API:**
     ```cpp
     BLEManager& ble = BLEManager::getInstance();
     ble.init();
     ble.startAdvertising();
     ble.sendData("Hello");
     ```

### 7. **Configuration Management System**
   - **Location:** `src/config_manager.h` and `src/config_manager.cpp`
   - **Features:**
     - Persistent JSON-based configuration storage
     - Device name customization
     - WiFi credentials storage
     - Power and display settings
     - Bluetooth enable/disable
     - Configuration save/load/reset
   - **Commands:**
     - `config show` - Display current configuration
     - `config save` - Save configuration to SD
     - `config reset` - Reset to defaults
   - **Default Configuration:**
     - Device Name: "M5 CardPuter"
     - Display Brightness: 200
     - Sleep Timeout: 60 seconds
     - Battery Monitoring: Enabled
     - Bluetooth: Enabled

## Console Commands Reference

### System Commands
- `help` - Show all available commands
- `sysinfo` - Display system information (heap size, chip info)

### WiFi Commands
- `wifi scan` - Scan and list available WiFi networks

### HID Commands
- `hid test` - Test keyboard and button functionality
- `hid info` - Show keyboard capabilities
- `hid mouse` - Mouse emulation information
- `hid gamepad` - Gamepad test mode

### Multitasking Commands
- `task list` - Show all active FreeRTOS tasks

### Power Management Commands
- `power info` - Display current power status
- `power battery` - Show battery voltage and percentage
- `power mode` - Display power mode information
- `power sleep` - Enter sleep mode

### Storage Commands
- `storage info` - Show SD card storage info
- `storage ls` - List contents of SD card

### Bluetooth Commands
- `ble start` - Initialize and start BLE advertising
- `ble stop` - Stop BLE advertising
- `ble info` - Show BLE connection status

### Configuration Commands
- `config show` - Display all current settings
- `config save` - Save settings to SD card
- `config reset` - Reset to default configuration

## Project Structure

```
src/
├── main.cpp                    # Main application loop
├── console.cpp/h              # Command console interface
├── multitask.cpp/h            # FreeRTOS task manager
├── display_manager.cpp/h      # Multi-window display system
├── power_manager.cpp/h        # Battery and power management
├── storage_manager.cpp/h      # SD card file operations
├── ble_manager.cpp/h          # Bluetooth/BLE interface
├── config_manager.cpp/h       # Configuration management
└── commands/                  # Command implementations
    ├── cmd_sys.cpp/h
    ├── cmd_wifi.cpp/h
    ├── cmd_hid.cpp/h
    ├── cmd_multitask.cpp/h
    ├── cmd_power.cpp/h
    ├── cmd_storage.cpp/h
    ├── cmd_ble.cpp/h
    └── cmd_config.cpp/h
```

## Building and Flashing

### Prerequisites
- PlatformIO IDE or CLI
- ESP-IDF toolchain (automatically installed by PlatformIO)

### Build
```bash
platformio run
```

### Upload to Device
```bash
platformio run --target upload
```

### Monitor Serial Output
```bash
platformio run --target monitor
```

### Combined Build, Upload, and Monitor
```bash
platformio run --target upload --target monitor
```

## Dependencies

The project uses the following libraries:
- **M5Cardputer** - M5 Stack hardware abstraction
- **ArduinoJson** - JSON configuration handling
- **NimBLE-Arduino** - Bluetooth/BLE support
- **FreeRTOS** - Task management and scheduling (built into ESP-IDF)

All dependencies are automatically managed by PlatformIO.

## Hardware Information

- **Device:** M5 Stack Card Puter S3A
- **Microcontroller:** ESP32-S3
- **Display:** 2.4" LCD (240x135)
- **Battery:** Built-in Li-Po battery with monitoring
- **Connectivity:** WiFi, Bluetooth/BLE
- **Storage:** MicroSD card slot
- **Input:** Physical keyboard and buttons

## Usage Examples

### Initialize All Systems
```cpp
// Done automatically in setup()
console_init();  // Initializes all managers
```

### Create a Task
```cpp
void myTaskFunction(void* param) {
    while(1) {
        // Task code
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

MultitaskManager& manager = MultitaskManager::getInstance();
manager.createTask("MyTask", myTaskFunction, nullptr, tskIDLE_PRIORITY + 1, TASK_USER);
```

### Write to Display Window
```cpp
DisplayManager& display = DisplayManager::getInstance();
int win = display.createWindow(10, 40, 220, 150, "Status");
display.writeToWindow(win, "Processing...");
```

### Check Battery Status
```cpp
PowerManager& power = PowerManager::getInstance();
if (power.getBatteryPercentage() < 20.0f) {
    power.setPowerMode(POWER_MODE_LOW_POWER);
}
```

### Store Configuration
```cpp
ConfigManager& config = ConfigManager::getInstance();
config.setDeviceName("MyCardPuter");
config.setBluetoothEnabled(true);
config.saveConfig();
```

### File Operations
```cpp
StorageManager& storage = StorageManager::getInstance();
if (storage.isSDCardMounted()) {
    storage.writeFile("/data.txt", "Hello World");
    String content = storage.readFile("/data.txt");
    storage.listDir("/");
}
```

## Performance Characteristics

- **Multitasking:** Up to 8 concurrent tasks with configurable priorities
- **Display:** Multi-window support with efficient rendering
- **Power:** Estimated battery life ~8-12 hours in normal mode, 24+ hours in low-power mode
- **Storage:** Full SD card support (typically 2GB-128GB)
- **Bluetooth:** BLE 5.0 with standard characteristics

## Future Enhancements

Potential additions for future versions:
- HTTP/HTTPS web server
- USB host/device mode
- Advanced graphics library
- File manager application
- Network protocols (MQTT, CoAP)
- Cryptographic operations
- Voice output (text-to-speech)
- Remote terminal access

## Troubleshooting

### SD Card Not Detected
- Check SD card is properly inserted
- Verify SD card is FAT32 formatted
- Try reformatting the SD card

### BLE Connection Issues
- Ensure BLE is enabled in configuration
- Check other Bluetooth devices aren't interfering
- Verify the remote device is compatible

### Battery Not Charging
- Check USB connection
- Verify battery is not completely dead
- May need 5-10 seconds for charging to begin

### High Power Consumption
- Check if sleep mode is functioning
- Reduce display brightness in configuration
- Switch to Low Power mode if not needed

## License

This firmware is provided as-is for use with M5 Stack hardware.

## Support

For issues or questions:
1. Check the console help: Type `help`
2. Review the command documentation above
3. Check hardware connections and SD card status

---

**Version:** 1.0.0
**Last Updated:** January 2026
**Compatible Hardware:** M5 Stack Card Puter S3A
