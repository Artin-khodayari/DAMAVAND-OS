#include "vga.hpp"
#include "isr.hpp"
#include "keyboard.hpp"
#include "banners.hpp"

#define fg 0xE // Yellow
#define bg 0   // Black
#define Command "#=>"

extern "C" void hlt_cpu();

static char line[256];
static unsigned len = 0;

// ===== Minimal string utils =====
static bool str_eq(const char* a, const char* b) {
    while (*a && *b) { if (*a != *b) return false; ++a; ++b; }
    return (*a == '\0' && *b == '\0');
}
static bool starts_with(const char* s, const char* prefix) {
    while (*prefix) { if (*s != *prefix) return false; ++s; ++prefix; }
    return true;
}

// ===== Command system =====
typedef void (*cmd_fn)(const char* args); // args points to first char after command (may be "")

// implementations (match your current behavior exactly)
static void cmd_help(const char*)   { HelpBanner(); }
static void cmd_clear(const char*)  { VGA::clear(); }
static void cmd_about(const char*)  { AboutBanner(); }
static void cmd_hello(const char*)  { VGA::setcolor(fg, bg); VGA::clear(); StartBanner(); }
static void cmd_reset(const char*)  { VGA::setcolor(fg, bg); }
static void cmd_echo(const char* args) {
    // current behavior: clear screen, print args, newline, switch to light magenta (0xD) on bg
    VGA::clear();
    VGA::puts(args);
    VGA::puts("\n");
    VGA::setcolor(0xD, bg);
}

// table of exact (single-word) commands
struct CommandEntry { const char* name; cmd_fn fn; };
static CommandEntry commands[] = {
    { "help",  cmd_help  },
    { "clear", cmd_clear },
    { "about", cmd_about },
    { "hello", cmd_hello },
    { "reset", cmd_reset },
};
static const int NUM_COMMANDS = sizeof(commands)/sizeof(commands[0]);

// dispatch a full line (null-terminated) to handlers
static void dispatch_line(const char* s) {
    // empty line → do nothing (your original behavior)
    if (*s == '\0') return;

    // special case with args: "echo <text>"
    if (starts_with(s, "echo ")) {
        cmd_echo(s + 5);
        return;
    }

    // exact matches
    for (int i = 0; i < NUM_COMMANDS; ++i) {
        if (str_eq(s, commands[i].name)) {
            commands[i].fn("");
            return;
        }
    }

    // unknown command
    VGA::puts("Unknown. try 'help'\n");
    VGA::setcolor(4, bg); // red on bg
}

// ===== keyboard callback =====
static void on_key(char c){
    if (c=='\n'){
        VGA::puts("\n");
        // make the buffer a proper C-string for our dispatcher
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

extern "C" void kmain(){
    VGA::init();
    StartBanner();
    VGA::puts("\n)>  To know how to work with DAMAVAND-OS, it's better to use 'help'.\n\n");

    ISR::install();
    Keyboard::set_callback(on_key);
    Keyboard::init();
    ISR::unmask_only_keyboard();

    asm volatile("sti");
    VGA::puts(Command);
    while (1) hlt_cpu();
}
