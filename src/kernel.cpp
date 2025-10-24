// src/kernel.cpp
#include "vga.hpp"
#include "isr.hpp"
#include "keyboard.hpp"
#include "banners.hpp"
#include "snake.hpp"

#define fg 0xE // Yellow
#define bg 0   // Black
#define Command "#=>"

extern "C" void hlt_cpu();

static char line[256];
static unsigned len = 0;

// === Minimal string utils ===
static bool str_eq(const char* a, const char* b) {
    while (*a && *b) { if (*a != *b) return false; ++a; ++b; }
    return (*a == '\0' && *b == '\0');
}
static bool starts_with(const char* s, const char* prefix) {
    while (*prefix) { if (*s != *prefix) return false; ++s; ++prefix; }
    return true;
}

// === Add this helper for outw ===
static inline void outw(u16 port, u16 val) {
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

// === Forward declaration of shell keyboard callback ===
static void on_key(char c);

// === Command system ===
typedef void (*cmd_fn)(const char* args); // ""

// implementations
static void cmd_help(const char*)   { HelpBanner(); }
static void cmd_clear(const char*)  { VGA::clear(); VGA::setcolor(fg, bg); }
static void cmd_about(const char*)  { AboutBanner(); }
static void cmd_hello(const char*)  { VGA::setcolor(fg, bg); VGA::clear(); StartBanner(); VGA::puts("\n");}
static void cmd_reset(const char*)  { VGA::setcolor(fg, bg); }
static void cmd_echo(const char* args) {
    VGA::clear();
    VGA::setcolor(fg, bg);
    VGA::puts(args);
    VGA::puts("\n");
}

// === Shutdown command ===
static void cmd_shutdown(const char*) {
    VGA::puts("\nShutting down...\n");

    // QEMU/Bochs quick power-off
    outw(0x604, 0x2000);

    // small delay
    for (volatile unsigned i = 0; i < 1000000; ++i) { asm volatile ("nop"); }

    // fallback halt
    VGA::puts("Power-off failed (no ACPI). Halting CPU.\n");
    asm volatile ("cli");
    while (1) hlt_cpu();
}
static void cmd_poweroff(const char*) { cmd_shutdown(nullptr); }

// === Snake command ===
static void cmd_snake(const char*) {
    VGA::puts("\nLaunching Snake... Use arrow keys. Press 'q' or ESC to quit.\n");
    VGA::clear();
    VGA::hideCursor();

    // Snake::run internally handles keyboard
    Snake::run();

    // Restore shell state
    len = 0;
    VGA::clear();
    VGA::setcolor(fg, bg);
    VGA::showCursor();
    VGA::puts("\nThanks for playing!\n");
}

// === Command table ===
struct CommandEntry { const char* name; cmd_fn fn; };
static CommandEntry commands[] = {
    { "help",     cmd_help  },
    { "clear",    cmd_clear },
    { "about",    cmd_about },
    { "hello",    cmd_hello },
    { "reset",    cmd_reset },
    { "poweroff", cmd_poweroff },
    { "down",     cmd_shutdown },
    { "snake",    cmd_snake },
};
static const int NUM_COMMANDS = sizeof(commands)/sizeof(commands[0]);

// === Dispatch shell command ===
static void dispatch_line(const char* s) {
    if (*s == '\0') return;

    if (starts_with(s, "echo ")) {
        cmd_echo(s + 5);
        return;
    }

    for (int i = 0; i < NUM_COMMANDS; ++i) {
        if (str_eq(s, commands[i].name)) {
            commands[i].fn("");
            return;
        }
    }

    VGA::puts("Unknown. try 'help'\n");
    VGA::setcolor(4, bg); // red on bg
}

// === Keyboard callback for shell ===
static void on_key(char c){
    if (c=='\n'){
        VGA::puts("\n");
        if (len < sizeof(line)) line[len] = '\0'; else line[sizeof(line)-1] = '\0';
        dispatch_line(line);
        len = 0;
        VGA::puts(Command);
        return;
    }
    if (c=='\b'){
        if (len){ --len; VGA::putc('\b'); }
        return;
    }
    if (len+1 < sizeof(line)){ line[len++]=c; VGA::putc(c); }
}

// === Main kernel entry point ===
extern "C" void kmain(){
    VGA::init();
    StartBanner();
    VGA::puts("\n)>  To know how to work with DAMAVAND-OS, use 'help'.\n\n");

    ISR::install();
    Keyboard::set_callback(on_key);
    Keyboard::init();
    ISR::unmask_only_keyboard();

    asm volatile("sti");
    VGA::puts(Command);

    while (1) hlt_cpu();
}
