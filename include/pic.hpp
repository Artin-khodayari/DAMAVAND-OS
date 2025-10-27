#pragma once
#include "types.hpp"
namespace PIC {
    void remap();                 // remap to 0x20..0x2F
    void set_mask(u16 mask);      // 16-bit mask (1 = masked)
    void send_eoi(int irq);       // irq 0..15
}
