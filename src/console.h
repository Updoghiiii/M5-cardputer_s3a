#pragma once
#include <Arduino.h>

void console_init();
void console_process_line(const String& line);
void console_println(const String& line);
void console_printf(const char* format, ...);
int console_get_line_count();
String console_get_line(int index);