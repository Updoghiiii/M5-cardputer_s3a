#include <M5Cardputer.h>
#include "console.h"
#include "multitask.h"
#include "display_manager.h"
#include "power_manager.h"

String buffer = "";

// Simple line history for scrolling console
static const int MAX_LINES = 32;
String consoleLines[MAX_LINES];
int lineCount = 0;

void console_add_line(const String& line) {
    if (lineCount < MAX_LINES) {
        consoleLines[lineCount++] = line;
    } else {
        for (int i = 1; i < MAX_LINES; ++i) {
            consoleLines[i - 1] = consoleLines[i];
        }
        consoleLines[MAX_LINES - 1] = line;
    }
}

void console_redraw() {
    auto &d = M5Cardputer.Display;
    d.fillScreen(BLACK);
    d.setTextSize(1);
    d.setCursor(0, 0);
    d.setTextWrap(false);

    const int VISIBLE_LINES = 15;
    int start = 0;

    if (lineCount > VISIBLE_LINES - 1) {
        start = lineCount - (VISIBLE_LINES - 1);
    }

    for (int i = start; i < lineCount; ++i) {
        d.println(consoleLines[i]);
    }

    d.print("> ");
    d.print(buffer);
}

void setup() {
    Serial.begin(115200);
    delay(500);

    auto cfg = M5.config();
    cfg.board = m5::board_t::board_M5Cardputer;
    cfg.output_power = true;
    cfg.internal_imu = false;
    cfg.internal_rtc = false;
    cfg.internal_spk = false;

    M5Cardputer.begin(cfg);

    DisplayManager::getInstance().init();
    console_init();
    PowerManager::getInstance().init();

    console_add_line("Cardputer Ready");
    console_add_line("Type 'help' for commands");
    console_redraw();
}

void loop() {
    M5Cardputer.update();
    PowerManager::getInstance().update();

    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            PowerManager::getInstance().resetIdleTimer();

            auto& state = M5Cardputer.Keyboard.keysState();
            bool changed = false;

            // ENTER
            if (state.enter) {
                console_add_line("> " + buffer);
                console_process_line(buffer);
                buffer = "";
                changed = true;
                Serial.println("[ENTER]");
            }
            // BACKSPACE
            else if (state.del) {
                if (buffer.length() > 0) {
                    buffer.remove(buffer.length() - 1);
                    changed = true;
                    Serial.println("[BACKSPACE]");
                }
            }
            // Process typed characters
            else if (state.word.size() > 0) {
                for (char c : state.word) {
                    if (c >= 32 && c <= 126) {  // printable ASCII
                        buffer += c;
                        changed = true;
                        Serial.printf("Char: '%c'\n", c);
                    }
                }
            }

            if (changed) {
                console_redraw();
            }
        }
    }

    delay(10);
}