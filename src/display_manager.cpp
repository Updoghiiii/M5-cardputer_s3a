#include "display_manager.h"

DisplayManager& DisplayManager::getInstance() {
    static DisplayManager instance;
    return instance;
}

DisplayManager::DisplayManager() : nextWindowId(0) {
}

void DisplayManager::init() {
    auto &d = M5Cardputer.Display;
    d.setTextColor(DEFAULT_TEXT_COLOR, DEFAULT_BG_COLOR);
    d.fillScreen(DEFAULT_BG_COLOR);
    Serial.println("Display Manager initialized");
}

void DisplayManager::update() {
    drawAllWindows();
}

int DisplayManager::createWindow(int x, int y, int width, int height, const char* title) {
    if (windows.size() >= MAX_WINDOWS) {
        Serial.println("Max windows reached");
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
    if (windowId >= 0 && windowId < (int)windows.size()) {
        auto &d = M5Cardputer.Display;
        Window_t &window = windows[windowId];
        
        d.setCursor(window.x + 5, window.y + 25);
        d.setTextColor(window.textColor, window.bgColor);
        d.println(text);
    }
}

void DisplayManager::clearWindow(int windowId) {
    if (windowId >= 0 && windowId < (int)windows.size()) {
        auto &d = M5Cardputer.Display;
        Window_t &window = windows[windowId];
        d.fillRect(window.x, window.y, window.width, window.height, window.bgColor);
    }
}

void DisplayManager::drawAllWindows() {
    auto &d = M5Cardputer.Display;
    for (size_t i = 0; i < windows.size(); i++) {
        if (windows[i].visible) {
            drawBorder(windows[i].x, windows[i].y, windows[i].width, windows[i].height, WHITE);
            
            d.setCursor(windows[i].x + 5, windows[i].y + 5);
            d.setTextColor(windows[i].textColor, DEFAULT_BG_COLOR);
            d.setTextSize(1);
            d.println(windows[i].title);
        }
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
    d.setTextColor(WHITE, BLACK);
    d.setCursor(10, 230);
    d.setTextSize(1);
    d.println(status);
}

void DisplayManager::drawBorder(int x, int y, int width, int height, uint16_t color) {
    auto &d = M5Cardputer.Display;
    d.drawRect(x, y, width, height, color);
}
