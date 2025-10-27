#pragma once
#include "types.hpp"
namespace ISR {
    typedef void (*irq_handler_t)();
    void install();                 // remap PIC, set IDT 0x20..0x2F, load IDT
    void register_irq(int n, irq_handler_t h);
    void unmask_only_keyboard();    // mask all except IRQ1
}
