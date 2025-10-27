#include "../include/keyboard.hpp"
#include "../include/isr.hpp"
#include "../include/ports.hpp"
#include "../include/vga.hpp"

namespace {
    Keyboard::key_cb_t g_cb = 0;

    const char sc_ascii[128] = {
        0, 27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
        '\t','Q','W','E','R','T','Y','U','I','O','P','[',']','\n', 0,
        'A','S','D','F','G','H','J','K','L',';','\'','`', 0,'\\',
        'Z','X','C','V','B','N','M',',','.','/', 0,'*', 0,' ', 0,0,0,0,0,0,0,0,0,0,0
    };

    void handler_irq1(){
        u8 sc = Ports::inb(0x60);
        if (sc & 0x80) return; // key release: ignore
        char ch = (sc < 128) ? sc_ascii[sc] : 0;
        if (!ch) return;
        if (g_cb) g_cb(ch); else VGA::puts("KBD(NO CB)\n");
    }
}

namespace Keyboard {
    void set_callback(key_cb_t cb){ g_cb = cb; }
    void init(){
        ISR::register_irq(1, handler_irq1);
    }
}
