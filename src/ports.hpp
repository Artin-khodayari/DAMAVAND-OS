#pragma once
#include "types.hpp"

namespace Ports {
    inline void outb(u16 port, u8 val){
        asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
    }
    inline u8 inb(u16 port){
        u8 ret; asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
        return ret;
    }
    inline void io_wait(){
        asm volatile ("outb %%al, $0x80" : : "a"(0));
    }
}
