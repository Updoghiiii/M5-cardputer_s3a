# M5‑Cardputer S3A System
Reconstructed, modular, operator‑centric firmware for the M5Stack Cardputer (ESP32‑S3).

## Project Overview
This firmware restores a clean, deduplicated architecture for the Cardputer S3A.  
It includes a full command console, subsystem managers, and hardware abstraction layers.

Subsystems:
- Power Manager
- Storage Manager
- WiFi Tools
- BLE Tools
- HID Tools
- Multitask Manager
- Config Manager
- System Info

## Commands

### Power
- power info  
- power battery  
- power mode  
- power mode <mode>  
- power sleep  

### Storage
- storage info  
- storage ls [path]  
- storage cat <file>  
- storage rm <file>  
- storage write <file> <text>  
- storage mkdir <dir>  
- storage rmdir <dir>  

### WiFi
- wifi scan  

### BLE
- ble scan  
- ble info  

### HID
- hid test  
- hid keyboard  
- hid mouse  
- hid gamepad  

### Multitask
- task list  
- task priority  

### Config
- config get <key>  
- config set <key> <value>  
- config list  

### System
- sysinfo  

## Architecture
src/
 ├─ main.cpp
 ├─ console/
 ├─ managers/
 ├─ commands/

## Build
PlatformIO handles dependencies automatically.

Build:
    pio run

Upload:
    pio run -t upload

## Console
Open serial at 115200 baud and issue commands directly.
