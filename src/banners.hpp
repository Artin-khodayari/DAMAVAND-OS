#pragma once
#include "vga.hpp"

// VGA text mode buffer starts at 0xb8000
unsigned short* VideoMemory = (unsigned short*)0xb8000;

// Cursor position
int cursorX = 0;
int cursorY = 0;

// VGA screen size
const int VGA_WIDTH = 80;
const int VGA_HEIGHT = 25;

// Current text color (default = white on black)
unsigned short textColor = 0x0F00;

// Write a character at position (x, y)
void putchar_at(char c, int x, int y) {
    VideoMemory[y * VGA_WIDTH + x] = textColor | c;
}

// Handle printing characters with cursor
void putchar(char c) {
    if (c == '\n') {
        cursorX = 0;
        cursorY++;
    } else {
        putchar_at(c, cursorX, cursorY);
        cursorX++;
        if (cursorX >= VGA_WIDTH) {
            cursorX = 0;
            cursorY++;
        }
    }

    // Scroll if needed
    if (cursorY >= VGA_HEIGHT) {
        // Move all rows up
        for (int y = 1; y < VGA_HEIGHT; y++) {
            for (int x = 0; x < VGA_WIDTH; x++) {
                VideoMemory[(y-1) * VGA_WIDTH + x] = VideoMemory[y * VGA_WIDTH + x];
            }
        }
        // Clear last line
        for (int x = 0; x < VGA_WIDTH; x++) {
            VideoMemory[(VGA_HEIGHT-1) * VGA_WIDTH + x] = textColor | ' ';
        }
        cursorY = VGA_HEIGHT - 1;
    }
}

// ==== Banners ====

void StartBanner(){
    unsigned short oldColor = textColor;
    VGA::puts(" ____  _____ _____ _____ _____ _____ _____ ____\n");  
    VGA::puts("|    \\|  _  |     |  _  |  |  |  _  |   | |    \\ \n");
    VGA::puts("|  |  |     | | | |     |  |  |     | | | |  |  |\n");
    VGA::puts("|____/|__|__|_|_|_|__|__|\\___/|__|__|_|___|____/\n");
    VGA::setcolor(0xE, 0);
    VGA::setCursor(0, cursorY + 3);
}

void HelpBanner() {
    unsigned short oldColor = textColor;
    VGA::puts("Commands:\n");
    VGA::puts("  clear --> Clears the screen\n");
    VGA::puts("  hello --> Showing the logo\n");
    VGA::puts("  about --> About OS & Developers\n");
    VGA::puts("  echo  --> print a message : echo <text>\n");
    VGA::puts("  help  --> ;)\n");
    VGA::setcolor(0xE, 0);
}

void AboutBanner() {
    unsigned short oldColor = textColor;
    VGA::puts("DAMAVAND-OS is an Open-source project.\n");
    VGA::puts("It's developed by Artin-khodayari.\n");
    VGA::puts("You can see the source code on GitHub:\n");
    VGA::puts("https://github.com/Artin-khodayari/DAMAVAND-OS\n");
    VGA::setcolor(0xE, 0);
}
