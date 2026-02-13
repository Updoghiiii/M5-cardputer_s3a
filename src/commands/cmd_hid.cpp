#include <Arduino.h>
#include <M5Cardputer.h>

void cmd_hid_test() {
    Serial.println("=== HID Test Mode ===");
    Serial.println("Press keys on the Cardputer keyboard.");
    Serial.println("Press ESC to exit.\n");

    auto &d = M5Cardputer.Display;
    d.fillScreen(BLACK);
    d.setCursor(0, 0);
    d.setTextSize(1);
    d.setTextColor(WHITE, BLACK);

    d.println("=== HID TEST MODE ===");
    d.println("Press keys...");
    d.println("----------------------");

    while (true) {
        M5Cardputer.update();

        auto &state = M5Cardputer.Keyboard.keysState();

        if (state.word.size() == 1 && state.word[0] == 27) {
            Serial.println("Exiting HID test mode.");
            break;
        }

        if (!M5Cardputer.Keyboard.isChange()) continue;

        d.fillRect(0, 40, d.width(), d.height() - 40, BLACK);
        d.setCursor(0, 40);

        d.println("Chars:");
        if (state.word.size() == 0) {
            d.println("  (none)");
        } else {
            for (char c : state.word) {
                d.printf("  '%c' (0x%02X)\n", c, c);
            }
        }

        d.println("\nHID Codes:");
        if (state.hid_keys.size() == 0) {
            d.println("  (none)");
        } else {
            for (uint8_t code : state.hid_keys) {
                d.printf("  0x%02X\n", code);
            }
        }

        d.println("\nModifiers:");
        d.printf("  Shift: %s\n", state.shift ? "ON" : "OFF");
        d.printf("  Ctrl : %s\n", state.ctrl ? "ON" : "OFF");
        d.printf("  Alt  : %s\n", state.alt ? "ON" : "OFF");
        d.printf("  Opt  : %s\n", state.opt ? "ON" : "OFF");
        d.printf("  Fn   : %s\n", state.fn ? "ON" : "OFF");
        d.printf("  Caps : %s\n", (state.modifiers & 0x02) ? "ON" : "OFF");

        delay(50);
    }

    d.fillScreen(BLACK);
    d.setCursor(0, 0);
    d.println("Exited HID Test Mode");
}

void cmd_hid_keyboard_info() {
    Serial.println("=== Keyboard Information ===");
    Serial.println("Keyboard Model: M5Cardputer");
    Serial.println("Layout: QWERTY");
    Serial.println("Keys: 47");
    Serial.println("Features:");
    Serial.println("  - Shift");
    Serial.println("  - Ctrl");
    Serial.println("  - Alt (Opt)");
    Serial.println("  - Function (Fn)");
    Serial.println("  - Caps Lock");
    Serial.println("  - HID Keycode Output");
    Serial.println("Status: Active\n");
}

void cmd_hid_mouse_emulate() {
    Serial.println("=== Mouse Emulation ===");
    Serial.println("Feature not implemented yet.");
    Serial.println("Planned: Arrow keys = move, Fn+Enter = click.\n");
}

void cmd_hid_gamepad_test() {
    Serial.println("=== Gamepad Test ===");
    Serial.println("Feature not implemented yet.");
    Serial.println("Planned: WASD + Enter + Space mapping.\n");
}