#pragma once
#include "types.hpp"

namespace IDT {
    struct Entry {
        u16 base_lo; u16 sel; u8 always0; u8 flags; u16 base_hi;
    } __attribute__((packed));

    struct Ptr {
        u16 limit; u32 base;
    } __attribute__((packed));

    void set_gate(u8 num, u32 base, u16 sel, u8 flags);
    void init();
}

extern "C" void idt_load(void* idtp);  // from isr_asm.s
