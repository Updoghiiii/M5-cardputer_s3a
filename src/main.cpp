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

    const int VISIBLE_LINES = 16;
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
    M5Cardputer.Keyboard.update();

    PowerManager::getInstance().update();

    auto& state = M5Cardputer.Keyboard.keysState();
    uint8_t key_count = M5Cardputer.Keyboard.isPressed();

    bool changed = false;

    // Only reset idle timer when actual keys are pressed
    if (key_count > 0 && key_count <= 10) {
        PowerManager::getInstance().resetIdleTimer();
    }

    // Ignore garbage
    if (key_count == 0 || key_count > 10) {
        delay(50);
        return;
    }

    // Process typed characters
    if (state.word.size() > 0 && state.word.size() <= 10) {
        for (char c : state.word) {
            if ((c >= 32 && c <= 126)) {  // printable ASCII
                buffer += c;
                changed = true;
                Serial.printf("Char: '%c'\n", c);
            }
        }
    }

    // ENTER
    if (state.enter && key_count <= 5) {
        console_add_line("> " + buffer);
        console_process_line(buffer);
        buffer = "";
        changed = true;
        Serial.println("[ENTER]");
    }

    // BACKSPACE
    if (state.del && key_count <= 5) {
        if (buffer.length() > 0) {
            buffer.remove(buffer.length() - 1);
            changed = true;
            Serial.println("[BACKSPACE]");
        }
    }

    if (changed) {
        console_redraw();
    }

    delay(50);
}