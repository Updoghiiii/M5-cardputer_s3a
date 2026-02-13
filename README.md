# M5 Card Puter S3A - Complete Firmware

Full-featured firmware for M5 Stack Card Puter S3A with multitasking, power management, storage, Bluetooth, and configuration.

## Features Implemented

✅ **HID Support** - Keyboard, buttons, mouse emulation
✅ **Multitasking** - FreeRTOS task management (up to 8 concurrent)
✅ **Display** - Multi-window system
✅ **Power** - Battery monitoring, 4 power modes
✅ **Storage** - SD card file operations
✅ **Bluetooth** - BLE connectivity
✅ **Configuration** - JSON persistent settings

## Quick Start

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

Baud rate: **115200**

## Console Commands

**System:**
- `help` - Show all commands
- `sysinfo` - System info

**Hardware:**
- `hid test` - Test keyboard/buttons
- `hid info` - Keyboard info
- `hid mouse` - Mouse emulation
- `hid gamepad` - Gamepad test

**WiFi:**
- `wifi scan` - Scan networks

**Tasks:**
- `task list` - Active tasks

**Power:**
- `power info` - Power status
- `power battery` - Battery voltage/percentage
- `power mode` - Power modes info
- `power sleep` - Sleep mode

**Storage:**
- `storage info` - SD card info
- `storage ls` - List SD contents

**Bluetooth:**
- `ble start` - Start advertising
- `ble stop` - Stop advertising
- `ble info` - BLE status

**Configuration:**
- `config show` - Display settings
- `config save` - Save to SD
- `config reset` - Reset to defaults

## Hardware

| Spec | Value |
|------|-------|
| Microcontroller | ESP32-S3 |
| Memory | 8 MB RAM, 16 MB Flash |
| Display | 2.4" LCD (240x135) |
| Battery | Built-in Li-Po |
| Keyboard | 47-key QWERTY |
| Connectivity | WiFi, BLE |
| Storage | MicroSD slot |

## Project Structure

```
src/
├── main.cpp
├── console.cpp/h
├── multitask.cpp/h          (FreeRTOS manager)
├── display_manager.cpp/h    (Multi-window display)
├── power_manager.cpp/h      (Battery management)
├── storage_manager.cpp/h    (SD card operations)
├── ble_manager.cpp/h        (Bluetooth/BLE)
├── config_manager.cpp/h     (Configuration)
└── commands/                (Command implementations)
```

## Build Status

✅ **Arduino Framework Only** (no ESP-IDF)
✅ **All Dependencies Included**
✅ **Ready to Upload**

## Dependencies

- M5Cardputer
- ArduinoJson
- NimBLE-Arduino

Auto-installed by PlatformIO.

## Troubleshooting

**Device not detected:**
- Check USB-C cable
- Try different USB port
- Verify device is powered on

**Commands not working:**
- Type `help` to verify syntax
- Check baud rate (115200)
- Ensure serial monitor is open

**SD Card not found:**
- Use FAT32 format
- Try reformatting
- Check proper insertion

## Version

- **Firmware:** 1.0.0
- **Date:** January 2026
- **Hardware:** M5 Stack Card Puter S3A
- **Framework:** Arduino
- **Language:** C++

---

**Status:** ✅ Complete and Ready for Deployment
