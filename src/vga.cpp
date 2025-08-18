#include "vga.hpp"

namespace {
    volatile u16* const VGA_MEM = (u16*)0xB8000;
    const u8 W = 80, H = 25;

    u8 cur_r = 0;   // current row
    u8 cur_c = 0;   // current column
    u8 fg    = 2;   // default = green
    u8 bg    = 0;   // default = black

    inline u16 cell(char ch) {
        return ((u16)bg << 12) | ((u16)fg << 8) | (u8)ch;
    }

    void scroll_if_needed() {
        if (cur_r < H) return;

        // scroll up 1 line
        for (u32 r = 1; r < H; ++r) {
            for (u32 c = 0; c < W; ++c) {
                VGA_MEM[(r - 1) * W + c] = VGA_MEM[r * W + c];
            }
        }

        // clear last line
        for (u32 c = 0; c < W; ++c)
            VGA_MEM[(H - 1) * W + c] = cell(' ');

        cur_r = H - 1;
    }
}

namespace VGA {
    void init() {
        clear();
    }
    // Clear the screen
    void clear() {
        for (u32 i = 0; i < (u32)W * (u32)H; ++i)
            VGA_MEM[i] = cell(' ');
        cur_r = 0;
        cur_c = 0;
    }
    // Only Chars
    void putc(char ch) {
        if (ch == '\n') {
            cur_c = 0;
            ++cur_r;
            scroll_if_needed();
            return;
        }
        if (ch == '\b') {
            if (cur_c > 0) {
                --cur_c;
                VGA_MEM[cur_r * W + cur_c] = cell(' ');
            }
            return;
        }

        VGA_MEM[cur_r * W + cur_c] = cell(ch);

        if (++cur_c >= W) {
            cur_c = 0;
            ++cur_r;
            scroll_if_needed();
        }
    }
    // Strings
    void puts(const char* s) {
        while (s && *s) {
            putc(*s++);
        }
    }
    // Change the screen color
    void repaint() {
        for (u32 r = 0; r < H; ++r) {
            for (u32 c = 0; c < W; ++c) {
                u16 entry = VGA_MEM[r * W + c];
                char ch = (char)(entry & 0x00FF); // keep character
                VGA_MEM[r * W + c] = cell(ch);    // rewrite with new fg/bg
            }
        }
    }
    // Final step of changing screen color
    void setcolor(u8 new_fg, u8 new_bg) {
        fg = new_fg;
        bg = new_bg;
        repaint();
    }

    // Move cursor to (x, y)
    void setCursor(int x, int y) {
        int cursorX = x;
        int cursorY = y;
    }

}
