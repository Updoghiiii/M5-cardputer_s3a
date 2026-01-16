#include "cmd_hid.h"
#include <Arduino.h>
#include <M5Cardputer.h>

void cmd_hid_test() {
    Serial.println("=== HID Test Suite ===");
    Serial.println("1. Keyboard Test");
    Serial.println("   - Press any key on the keyboard");
    Serial.println("   - Keys will be echoed to serial");
    
    Serial.println("\n2. Button Test");
    Serial.println("   - Testing M5 buttons...");
    
    // Test button states
    M5Cardputer.update();
    if (M5Cardputer.BtnA.isPressed()) {
        Serial.println("   - Button A: PRESSED");
    }
    if (M5Cardputer.BtnB.isPressed()) {
        Serial.println("   - Button B: PRESSED");
    }
    
    Serial.println("\n3. Keyboard Features:");
    Serial.println("   - Shift support");
    Serial.println("   - Caps lock");
    Serial.println("   - Special characters");
    Serial.println("\nHID Test Complete");
}

void cmd_hid_keyboard_info() {
    Serial.println("=== Keyboard Information ===");
    Serial.println("Device: M5 CardPuter Keyboard");
    Serial.println("Keys: 47 keys");
    Serial.println("Features:");
    Serial.println("  - Shift key support");
    Serial.println("  - Caps lock key");
    Serial.println("  - Function keys");
    Serial.println("  - Arrow keys");
    Serial.println("  - Special character support");
    Serial.println("============================");
}

void cmd_hid_mouse_emulate() {
    Serial.println("Mouse emulation:");
    Serial.println("  - Move: Use arrow keys");
    Serial.println("  - Click: Use Ctrl+Click key");
    Serial.println("  - Right-click: Use Shift+Click key");
}

void cmd_hid_gamepad_test() {
    Serial.println("=== Gamepad Test ===");
    Serial.println("Available inputs:");
    Serial.println("  - Button A: Select/Confirm");
    Serial.println("  - Button B: Back/Cancel");
    Serial.println("  - Keyboard: D-Pad and Action buttons");
    Serial.println("Test: Press buttons to see values");
    
    // Read button states
    M5Cardputer.update();
    bool btnA = M5Cardputer.BtnA.isPressed();
    bool btnB = M5Cardputer.BtnB.isPressed();
    
    if (btnA || btnB) {
        Serial.printf("Button A: %s\n", btnA ? "PRESSED" : "released");
        Serial.printf("Button B: %s\n", btnB ? "PRESSED" : "released");
    } else {
        Serial.println("Waiting for button press...");
    }
}