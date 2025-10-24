#include "vga.hpp"

namespace {
    // types.hpp is assumed to define u8/u16/u32
    volatile u16* const VGA_MEM = (u16*)0xB8000;
    const u8 W = 80, H = 25;

    u8 cur_r = 0;   // current row
    u8 cur_c = 0;   // current column
    u8 fg    = 2;   // default = green
    u8 bg    = 0;   // default = black

    bool cursor_visible = true;

    inline u16 cell(char ch) {
        // attribute byte = (bg << 4) | (fg & 0x0F)
        u16 attr = ((u16)bg << 4) | ((u16)fg & 0x0F);
        return (attr << 8) | (u8)ch;
    }

    static inline void outb(u16 port, u8 val) {
        asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
    }
    static inline u8 inb(u16 port) {
        u8 ret;
        asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
        return ret;
    }

    // write hardware cursor position
    void move_hardware_cursor() {
        // always update position registers
        u16 pos = (u16)cur_r * (u16)W + (u16)cur_c;
        outb(0x3D4, 0x0F);
        outb(0x3D5, (u8)(pos & 0xFF));
        outb(0x3D4, 0x0E);
        outb(0x3D5, (u8)((pos >> 8) & 0xFF));
    }

    // set/clear bit 5 of CRTC register 0x0A to hide/show cursor
    void set_hardware_cursor_visibility(bool visible) {
        // select register 0x0A and read its value
        outb(0x3D4, 0x0A);
        u8 val = inb(0x3D5);
        if (visible) val &= ~0x20; else val |= 0x20;
        outb(0x3D4, 0x0A);
        outb(0x3D5, val);
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
        showCursor(); // default visible
    }

    // Clear the screen
    void clear() {
        for (u32 i = 0; i < (u32)W * (u32)H; ++i)
            VGA_MEM[i] = cell(' ');
        cur_r = 0;
        cur_c = 0;
        move_hardware_cursor();
    }

    // Only Chars
    void putc(char ch) {
        if (ch == '\n') {
            cur_c = 0;
            ++cur_r;
            scroll_if_needed();
            move_hardware_cursor();
            return;
        }
        if (ch == '\b') {
            if (cur_c > 0) {
                --cur_c;
                VGA_MEM[cur_r * W + cur_c] = cell(' ');
            }
            move_hardware_cursor();
            return;
        }

        VGA_MEM[cur_r * W + cur_c] = cell(ch);

        if (++cur_c >= W) {
            cur_c = 0;
            ++cur_r;
            scroll_if_needed();
        }

        move_hardware_cursor();
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
        move_hardware_cursor();
    }

    // Final step of changing screen color
    void setcolor(u8 new_fg, u8 new_bg) {
        fg = new_fg;
        bg = new_bg;
        repaint();
    }

    // Move cursor to (x, y)
    // x = column (0..W-1), y = row (0..H-1)
    void setCursor(int x, int y) {
        if (x < 0) x = 0;
        if (x >= W) x = W - 1;
        if (y < 0) y = 0;
        if (y >= H) y = H - 1;
        cur_c = (u8)x;
        cur_r = (u8)y;
        move_hardware_cursor();
    }

    // === Public cursor control API ===
    void hideCursor() {
        cursor_visible = false;
        set_hardware_cursor_visibility(false);
    }

    void showCursor() {
        cursor_visible = true;
        set_hardware_cursor_visibility(true);
        move_hardware_cursor();
    }

    void setCursorVisible(bool visible) {
        if (visible) showCursor(); else hideCursor();
    }
}
