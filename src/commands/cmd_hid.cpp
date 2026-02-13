#include <Arduino.h>
#include <M5Cardputer.h>
#include "../console.h"

void cmd_hid_test() {
    console_println("=== HID Test Mode ===");
    console_println("Press keys on the Cardputer keyboard.");
    console_println("Press ESC to exit.\n");

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
            console_println("Exiting HID test mode.");
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
    console_println("=== Keyboard Information ===");
    console_println("Keyboard Model: M5Cardputer");
    console_println("Layout: QWERTY");
    console_println("Keys: 47");
    console_println("Features:");
    console_println("  - Shift");
    console_println("  - Ctrl");
    console_println("  - Alt (Opt)");
    console_println("  - Function (Fn)");
    console_println("  - Caps Lock");
    console_println("  - HID Keycode Output");
    console_println("Status: Active\n");
}

void cmd_hid_mouse_emulate() {
    console_println("=== Mouse Emulation ===");
    console_println("Feature not implemented yet.");
    console_println("Planned: Arrow keys = move, Fn+Enter = click.\n");
}

void cmd_hid_gamepad_test() {
    console_println("=== Gamepad Test ===");
    console_println("Feature not implemented yet.");
    console_println("Planned: WASD + Enter + Space mapping.\n");
}