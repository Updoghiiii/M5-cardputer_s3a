#pragma once
#include <M5Cardputer.h>
#include <vector>

#define MAX_WINDOWS 4
#define DEFAULT_BG_COLOR BLACK
#define DEFAULT_TEXT_COLOR WHITE

typedef struct {
    int x, y, width, height;
    uint16_t bgColor;
    uint16_t textColor;
    const char* title;
    bool visible;
    int scrollOffset;
} Window_t;

class DisplayManager {
public:
    static DisplayManager& getInstance();
    
    void init();
    void update();
    
    int createWindow(int x, int y, int width, int height, const char* title);
    void closeWindow(int windowId);
    void writeToWindow(int windowId, const String& text);
    void clearWindow(int windowId);
    void drawAllWindows();
    void setWindowVisible(int windowId, bool visible);
    
    void drawStatusBar(const String& status);
    void drawBorder(int x, int y, int width, int height, uint16_t color);
    
private:
    DisplayManager();
    std::vector<Window_t> windows;
    String statusBar;
    int nextWindowId;
};
