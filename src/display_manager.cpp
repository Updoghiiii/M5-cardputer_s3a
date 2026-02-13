#include "display_manager.h"
#include <M5Cardputer.h>

DisplayManager& DisplayManager::getInstance() {
    static DisplayManager instance;
    return instance;
}

DisplayManager::DisplayManager()
    : nextWindowId(0) {
}

#include "console.h"
void DisplayManager::init() {
    auto &d = M5Cardputer.Display;
    d.fillScreen(DEFAULT_BG_COLOR);
    d.setTextColor(DEFAULT_TEXT_COLOR, DEFAULT_BG_COLOR);
    d.setTextSize(1);
    console_println("Display Manager initialized");
}

void DisplayManager::update() {
    drawAllWindows();
}

int DisplayManager::createWindow(int x, int y, int width, int height, const char* title) {
    if (windows.size() >= MAX_WINDOWS) {
        console_println("Max windows reached");
        return -1;
    }

    Window_t window;
    window.x = x;
    window.y = y;
    window.width = width;
    window.height = height;
    window.title = title;
    window.visible = true;
    window.bgColor = BLACK;
    window.textColor = WHITE;
    window.scrollOffset = 0;

    windows.push_back(window);
    return nextWindowId++;
}

void DisplayManager::closeWindow(int windowId) {
    if (windowId >= 0 && windowId < (int)windows.size()) {
        windows.erase(windows.begin() + windowId);
    }
}

void DisplayManager::writeToWindow(int windowId, const String& text) {
    if (windowId < 0 || windowId >= (int)windows.size()) return;

    auto &d = M5Cardputer.Display;
    Window_t &window = windows[windowId];

    int textX = window.x + 5;
    int textY = window.y + 20;

    d.fillRect(window.x + 1, window.y + 15, window.width - 2, window.height - 16, window.bgColor);
    d.setCursor(textX, textY);
    d.setTextColor(window.textColor, window.bgColor);
    d.println(text);
}

void DisplayManager::clearWindow(int windowId) {
    if (windowId < 0 || windowId >= (int)windows.size()) return;

    auto &d = M5Cardputer.Display;
    Window_t &window = windows[windowId];

    d.fillRect(window.x, window.y, window.width, window.height, window.bgColor);
}

void DisplayManager::drawAllWindows() {
    auto &d = M5Cardputer.Display;

    for (size_t i = 0; i < windows.size(); i++) {
        if (!windows[i].visible) continue;

        Window_t &w = windows[i];

        drawBorder(w.x, w.y, w.width, w.height, WHITE);

        d.fillRect(w.x + 1, w.y + 1, w.width - 2, 14, DEFAULT_BG_COLOR);

        d.setCursor(w.x + 4, w.y + 3);
        d.setTextColor(w.textColor, DEFAULT_BG_COLOR);
        d.setTextSize(1);
        d.println(w.title);
    }
}

void DisplayManager::setWindowVisible(int windowId, bool visible) {
    if (windowId >= 0 && windowId < (int)windows.size()) {
        windows[windowId].visible = visible;
    }
}

void DisplayManager::drawStatusBar(const String& status) {
    auto &d = M5Cardputer.Display;

    statusBar = status;

    d.fillRect(0, d.height() - 12, d.width(), 12, BLACK);
    d.setTextColor(WHITE, BLACK);
    d.setCursor(5, d.height() - 10);
    d.setTextSize(1);
    d.println(status);
}

void DisplayManager::drawBorder(int x, int y, int width, int height, uint16_t color) {
    auto &d = M5Cardputer.Display;
    d.drawRect(x, y, width, height, color);
}