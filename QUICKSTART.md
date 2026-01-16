# M5 Card Puter S3A - Quick Start Guide

## Initial Setup

1. **Connect the Device**
   - Plug the M5 Card Puter S3A into your computer via USB-C
   - Ensure the device is powered on

2. **Select the COM Port**
   - In PlatformIO: Run → Select Default Environment → m5stack-stamps3
   - PlatformIO will auto-detect the COM port

3. **Build and Upload**
   ```bash
   platformio run --target upload --target monitor
   ```

4. **Verify Installation**
   - You should see: "Cardputer Ready" on the display
   - Type `help` in the serial monitor to see available commands

## First Commands to Try

### 1. Check System Status
```
sysinfo
```
Shows ESP32-S3 memory and system info.

### 2. Scan WiFi Networks
```
wifi scan
```
Lists available WiFi networks nearby.

### 3. Check Battery
```
power battery
```
Shows battery voltage and estimated charge percentage.

### 4. Test Hardware
```
hid test
```
Tests keyboard, buttons, and input devices.

### 5. View Configuration
```
config show
```
Displays all current device settings.

## Key Features Overview

### Multitasking
The device now supports FreeRTOS multitasking! Multiple operations can run concurrently:
- View active tasks: `task list`
- The firmware manages WiFi, BLE, storage, and power monitoring simultaneously

### Power Management
Smart battery management automatically optimizes power consumption:
- Normal mode: Full performance
- Efficiency mode: Reduced power use
- Low Power mode: Minimal functions (auto-activates at 20% battery)
- Sleep mode: Deep sleep (activate with `power sleep`)

### Bluetooth Connectivity
Wireless device control and communication:
- Start advertising: `ble start`
- Check status: `ble info`
- Automatically discoverable as "M5 CardPuter"

### File Storage
Access the built-in microSD card:
- View storage info: `storage info`
- List files: `storage ls`
- Configuration saved to `/config.json`

### Custom Configuration
Save your preferences:
- View settings: `config show`
- Save changes: `config save`
- Reset to defaults: `config reset`

## Serial Monitor Usage

1. Open Serial Monitor in PlatformIO
2. Set baud rate to **115200**
3. Type commands and press Enter
4. Use keyboard on device to enter commands and see output

### Keyboard Controls
- **Letters/Numbers** - Normal input
- **Shift** - Hold for uppercase/special chars
- **Caps Lock** - Toggle uppercase mode
- **Enter** - Execute command
- **Backspace** - Delete character
- **Escape** - Clear input

## Troubleshooting

### No Serial Output
- Check USB cable connection
- Verify COM port in device manager
- Try different USB port
- Restart PlatformIO

### Commands Not Recognized
- Type `help` to see all available commands
- Check spelling (case-sensitive)
- Press Enter after typing command

### Display Not Showing Text
- Verify M5Cardputer library is installed
- Check hardware connections
- Upload firmware again

### WiFi Scan Returns Nothing
- Ensure WiFi networks are available in range
- Check WiFi is enabled on nearby devices
- May need to be closer to routers

## Next Steps

1. **Explore Commands** - Try each command from the help menu
2. **Insert SD Card** - Add a microSD card for file storage
3. **Enable Bluetooth** - Connect wireless devices
4. **Customize Settings** - Adjust brightness, timeouts, etc.
5. **Develop Applications** - Extend firmware with your own tasks

## Tips & Tricks

- **Battery Optimization:** Enable Low Power mode to extend battery life
- **Offline Operation:** All features work without WiFi
- **Quick Commands:** Short commands like `help`, `sysinfo`, `power info`
- **Status Bar:** Device automatically shows battery level
- **Persistent Settings:** Changes made with `config` are saved to SD card

## Default Credentials

When connecting via Bluetooth, the device identifies itself as:
- **Name:** M5 CardPuter
- **Service UUID:** 180A
- **Characteristic UUID:** 2A29

## Support Commands

```
help              - Show all commands
config show       - Display settings
power info        - Check battery
task list         - View running tasks
storage info      - SD card status
hid test          - Verify input devices
ble info          - Bluetooth status
```

## Getting Help

If something doesn't work:
1. Type `help` to verify command syntax
2. Check `config show` for settings
3. Run `sysinfo` to check system health
4. Try `power info` to check battery status
5. Review the FIRMWARE_FEATURES.md for detailed documentation

---

**Ready to use!** Your M5 Card Puter S3A is now fully functional with multitasking support.
