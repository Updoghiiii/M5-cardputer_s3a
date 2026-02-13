#include <M5Cardputer.h>
#include "console.h"
#include "multitask.h"
#include "display_manager.h"
#include "power_manager.h"
#include <vector>

String buffer = "";
int cursor_pos = 0;
std::vector<String> history;
int history_idx = -1;

void console_redraw() {
    auto &d = M5Cardputer.Display;
    d.fillScreen(BLACK);
    d.setTextSize(1);
    d.setCursor(0, 0);
    d.setTextWrap(false);

    const int VISIBLE_LINES = 15;
    int start = 0;
    int lineCount = console_get_line_count();

    if (lineCount > VISIBLE_LINES - 1) {
        start = lineCount - (VISIBLE_LINES - 1);
    }

    for (int i = start; i < lineCount; ++i) {
        d.println(console_get_line(i));
    }

    d.print("> ");
    String before = buffer.substring(0, cursor_pos);
    String at = (cursor_pos < buffer.length()) ? String(buffer[cursor_pos]) : " ";
    String after = (cursor_pos < buffer.length()) ? buffer.substring(cursor_pos + 1) : "";

    d.print(before);

    // Draw cursor
    uint16_t curX = d.getCursorX();
    uint16_t curY = d.getCursorY();
    d.fillRect(curX, curY, 6, 8, WHITE);
    d.setTextColor(BLACK, WHITE);
    d.print(at);
    d.setTextColor(WHITE, BLACK);

    d.print(after);
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

    console_println("Cardputer Ready");
    console_println("Type 'help' for commands");
    console_redraw();
}

unsigned long lastStatusUpdate = 0;

void loop() {
    M5Cardputer.update();
    PowerManager::getInstance().update();

    if (millis() - lastStatusUpdate > 1000) {
        lastStatusUpdate = millis();
        char status[64];
        float battery = PowerManager::getInstance().getBatteryPercentage();
        snprintf(status, sizeof(status), "BAT: %.0f%% | HEAP: %u KB", battery, esp_get_free_heap_size() / 1024);
        DisplayManager::getInstance().drawStatusBar(status);
    }

    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            PowerManager::getInstance().resetIdleTimer();

            auto& state = M5Cardputer.Keyboard.keysState();
            bool changed = false;

            // Arrows via Fn
            if (state.fn) {
                for (char c : state.word) {
                    if (c == ';') { // UP
                        if (history_idx < (int)history.size() - 1) {
                            history_idx++;
                            buffer = history[history.size() - 1 - history_idx];
                            cursor_pos = buffer.length();
                            changed = true;
                        }
                    } else if (c == '.') { // DOWN
                        if (history_idx > 0) {
                            history_idx--;
                            buffer = history[history.size() - 1 - history_idx];
                            cursor_pos = buffer.length();
                            changed = true;
                        } else if (history_idx == 0) {
                            history_idx = -1;
                            buffer = "";
                            cursor_pos = 0;
                            changed = true;
                        }
                    } else if (c == ',') { // LEFT
                        if (cursor_pos > 0) {
                            cursor_pos--;
                            changed = true;
                        }
                    } else if (c == '/') { // RIGHT
                        if (cursor_pos < (int)buffer.length()) {
                            cursor_pos++;
                            changed = true;
                        }
                    }
                }
            }
            // ENTER
            else if (state.enter) {
                if (buffer.length() > 0) {
                    history.push_back(buffer);
                    if (history.size() > 20) history.erase(history.begin());
                }
                history_idx = -1;

                console_println("> " + buffer);
                console_process_line(buffer);
                buffer = "";
                cursor_pos = 0;
                changed = true;
                Serial.println("[ENTER]");
            }
            // BACKSPACE
            else if (state.del) {
                if (cursor_pos > 0) {
                    buffer.remove(cursor_pos - 1, 1);
                    cursor_pos--;
                    changed = true;
                    Serial.println("[BACKSPACE]");
                }
            }
            // Process typed characters
            else if (state.word.size() > 0) {
                for (char c : state.word) {
                    if (c >= 32 && c <= 126) {  // printable ASCII
                        buffer = buffer.substring(0, cursor_pos) + c + buffer.substring(cursor_pos);
                        cursor_pos++;
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