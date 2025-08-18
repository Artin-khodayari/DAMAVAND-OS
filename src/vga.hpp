#pragma once
#include "types.hpp"

namespace VGA {
    void init();            // initialize driver
    void clear();           // clear screen
    void putc(char ch);     // print one character
    void puts(const char*); // print string

    void repaint();                     // repaint whole screen with current fg/bg
    void setcolor(u8 new_fg, u8 new_bg); // set fg/bg and repaint

    // move the software cursor
    void setCursor(int x, int y);
}
