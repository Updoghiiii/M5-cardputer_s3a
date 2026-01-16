#include <M5Cardputer.h>
#include "console.h"
#include "multitask.h"
#include "display_manager.h"
#include "power_manager.h"

String buffer = "";

void setup() {
    Serial.begin(115200);
    M5Cardputer.begin();
    
    // Initialize all managers
    DisplayManager::getInstance().init();
    console_init();
    PowerManager::getInstance().update();
    
    auto &d = M5Cardputer.Display;
    d.setTextSize(2);
    d.setCursor(10, 20);
    d.println("Cardputer Ready");
    d.setTextSize(1);
    d.setCursor(10, 50);
    d.println("Type 'help' for commands");
}

void loop() {
    M5Cardputer.update();
    
    // Update power management
    PowerManager::getInstance().update();
    PowerManager::getInstance().resetIdleTimer();
    
    if (!M5Cardputer.Keyboard.isChange()) {
        return;
    }

    const auto& keys = M5Cardputer.Keyboard.keyList();
    auto& state = M5Cardputer.Keyboard.keysState();

    for (const auto& coord : keys) {
        KeyValue_t kv = M5Cardputer.Keyboard.getKeyValue(coord);
        char ascii = kv.value_first;

        if (state.shift || M5Cardputer.Keyboard.capslocked()) {
            ascii = kv.value_second;
        }

        if (ascii == 13) {
            console_process_line(buffer);
            buffer = "";
            M5Cardputer.Display.println();
            continue;
        }

        if (ascii == 8) {
            if (buffer.length() > 0) {
                buffer.remove(buffer.length() - 1);
                M5Cardputer.Display.fillScreen(BLACK);
                M5Cardputer.Display.setCursor(10, 20);
                M5Cardputer.Display.setTextSize(2);
                M5Cardputer.Display.print(buffer);
            }
            continue;
        }

        if (ascii == 9) {
            buffer += "\t";
            M5Cardputer.Display.print("    ");
            continue;
        }

        if (ascii == 27) {
            buffer = "";
            M5Cardputer.Display.fillScreen(BLACK);
            M5Cardputer.Display.setCursor(10, 20);
            M5Cardputer.Display.setTextSize(2);
            continue;
        }

        if (ascii > 31 && ascii < 127) {
            buffer += ascii;
            M5Cardputer.Display.print(ascii);
        }
    }
}