#include "../include/vga.hpp"
#include "../include/isr.hpp"
#include "../include/keyboard.hpp"
#include "../include/banners.hpp"
#include "../include/snake.hpp"

#define Command "#=>"

extern "C" void hlt_cpu();

static char line[256];
static unsigned len = 0;

// Mutable colors
static unsigned char fg = 0xE; // Yellow
static unsigned char bg = 0x0; // Black

// Minimal string utils
static bool str_eq(const char* a, const char* b) {
    while (*a && *b) { if (*a != *b) return false; ++a; ++b; }
    return (*a == '\0' && *b == '\0');
}
static bool starts_with(const char* s, const char* prefix) {
    while (*prefix) { if (*s != *prefix) return false; ++s; ++prefix; }
    return true;
}
static const char* skip_spaces(const char* s) {
    while (*s == ' ') ++s;
    return s;
}

// Small integer parsing
static int to_int(const char* s) {
    s = skip_spaces(s);
    if (!(*s >= '0' && *s <= '9')) return -1;
    int n = 0;
    while (*s >= '0' && *s <= '9') { n = n * 10 + (*s - '0'); ++s; }
    return n;
}

// Add this helper for outw
static inline void outw(u16 port, u16 val) {
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

// Forward declaration of shell keyboard callback
static void on_key(char c);

// Command system
typedef void (*cmd_fn)(const char* args); // ""

// implementations
static void cmd_help(const char*)   { HelpBanner(); }
static void cmd_clear(const char*)  { VGA::clear(); VGA::setcolor(fg, bg); }
static void cmd_about(const char*)  { AboutBanner(); }
static void cmd_hello(const char*)  { VGA::setcolor(fg, bg); VGA::clear(); StartBanner(); VGA::puts("\n");}
static void cmd_reset(const char*)  { VGA::setcolor(fg, bg); }
static void cmd_echo(const char* args) {
    VGA::setcolor(fg, bg);
    VGA::puts(args);
    VGA::puts("\n");
}

// Shutdown command
static void cmd_shutdown(const char*) {
    VGA::puts("\nSHUTTING DOWN...\n");

    // QEMU/Bochs quick power-off
    outw(0x604, 0x2000);

    // small delay
    for (volatile unsigned i = 0; i < 1000000; ++i) { asm volatile ("nop"); }

    // fallback halt
    VGA::puts("POWER-OFF FAILED (NO ACPI). HALTING CPU.\n");
    asm volatile ("cli");
    while (1) hlt_cpu();
}
static void cmd_poweroff(const char*) { cmd_shutdown(nullptr); }

// Snake command
static void cmd_snake(const char*) {
    VGA::puts("\nLAUNCHING SNAKE... USE ARROW KEYS. PRESS 'Q' OR ESC TO QUIT.\n");
    VGA::clear();
    VGA::hideCursor();

    // Snake::run internally handles keyboard
    Snake::run();

    // Restore shell state
    len = 0;
    VGA::clear();
    VGA::setcolor(fg, bg);
    VGA::showCursor();
    VGA::puts("\nTHANKS FOR PLAYING!\n");
}

// Color commands (FG, BG, COLOR)
static void cmd_set_fg(const char* args) {
    int v = to_int(args);
    if (v < 0 || v > 15) {
        VGA::puts("USAGE: FG <0-15>\n");
        return;
    }
    fg = (unsigned char)v;
    VGA::setcolor(fg, bg);
    VGA::puts("FG SET.\n");
}

static void cmd_set_bg(const char* args) {
    int v = to_int(args);
    if (v < 0 || v > 15) {
        VGA::puts("USAGE: BG <0-15>\n");
        return;
    }
    bg = (unsigned char)v;
    VGA::setcolor(fg, bg);
    VGA::puts("BG SET.\n");
}

static void cmd_color(const char* args) {
    // COLOR <fg> [bg]
    const char* p = skip_spaces(args);
    int new_fg = to_int(p);
    if (new_fg < 0 || new_fg > 15) {
        VGA::puts("USAGE: COLOR <fg 0-15> [bg 0-15]\n");
        return;
    }
    // advance to next token
    while (*p && *p != ' ') ++p;
    p = skip_spaces(p);
    int new_bg = to_int(p);
    if (new_bg < 0) new_bg = bg; // keep existing if not provided
    if (new_bg < 0 || new_bg > 15) {
        VGA::puts("BG MUST BE 0-15.\n");
        return;
    }
    fg = (unsigned char)new_fg;
    bg = (unsigned char)new_bg;
    VGA::setcolor(fg, bg);
    VGA::puts("COLOR UPDATED.\n");
}

// Command table
struct CommandEntry { const char* name; cmd_fn fn; };
static CommandEntry commands[] = {
    { "HELP",     cmd_help  },
    { "CLEAR",    cmd_clear },
    { "ABOUT",    cmd_about },
    { "HELLO",    cmd_hello },
    { "RESET",    cmd_reset },
    { "OFF",      cmd_poweroff },
    { "DOWN",     cmd_shutdown },
    { "SNAKE",    cmd_snake },
    { "COLOR",    cmd_color },
    { "FG",       cmd_set_fg },
    { "BG",       cmd_set_bg },
};
static const int NUM_COMMANDS = sizeof(commands)/sizeof(commands[0]);

// Dispatch shell command
static void dispatch_line(const char* s) {
    if (*s == '\0') return;

    if (starts_with(s, "ECHO ")) {
        cmd_echo(s + 5);
        return;
    }

    // direct handling for FG/BG/COLOR to accept args in flexible form
    // start
    if (starts_with(s, "FG")) {
        const char* args = s + 2;
        args = skip_spaces(args);
        cmd_set_fg(args);
        return;
    }
    if (starts_with(s, "BG")) {
        const char* args = s + 2;
        args = skip_spaces(args);
        cmd_set_bg(args);
        return;
    }
    // end

    if (starts_with(s, "COLOR")) {
        const char* args = s + 5;
        args = skip_spaces(args);
        cmd_color(args);
        return;
    }

    // exact-match commands (no args)
    for (int i = 0; i < NUM_COMMANDS; ++i) {
        if (str_eq(s, commands[i].name)) {
            commands[i].fn("");
            return;
        }
    }

    VGA::puts("UNKNOWN. TRY 'HELP'\n");
    VGA::setcolor(4, bg); // red on bg
}

// Keyboard callback for shell
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

// Main kernel entry point
extern "C" void kmain(){
    VGA::init();
    StartBanner();
    VGA::puts("\n)>  TO KNOW HOW TO WORK WITH DAMAVAND-OS, USE 'HELP'.\n\n");

    ISR::install();
    Keyboard::set_callback(on_key);
    Keyboard::init();
    ISR::unmask_only_keyboard();

    asm volatile("sti");
    VGA::setcolor(fg, bg);
    VGA::puts(Command);

    while (1) hlt_cpu();
}
