#include "../include/pic.hpp"
#include "../include/ports.hpp"

namespace {
    const u8 PIC1=0x20, PIC2=0xA0;
    const u8 PIC1_CMD=PIC1, PIC1_DATA=0x21;
    const u8 PIC2_CMD=PIC2, PIC2_DATA=0xA1;
    const u8 ICW1_INIT=0x10, ICW1_ICW4=0x01;
    const u8 ICW4_8086=0x01;
    u16 g_mask = 0xFFFF;
}
namespace PIC {
    void remap(){
        u8 a1 = Ports::inb(PIC1_DATA);
        u8 a2 = Ports::inb(PIC2_DATA);

        Ports::outb(PIC1_CMD, ICW1_INIT|ICW1_ICW4); Ports::io_wait();
        Ports::outb(PIC2_CMD, ICW1_INIT|ICW1_ICW4); Ports::io_wait();

        Ports::outb(PIC1_DATA, 0x20); Ports::io_wait();
        Ports::outb(PIC2_DATA, 0x28); Ports::io_wait();

        Ports::outb(PIC1_DATA, 4); Ports::io_wait();
        Ports::outb(PIC2_DATA, 2); Ports::io_wait();

        Ports::outb(PIC1_DATA, ICW4_8086); Ports::io_wait();
        Ports::outb(PIC2_DATA, ICW4_8086); Ports::io_wait();

        Ports::outb(PIC1_DATA, a1);
        Ports::outb(PIC2_DATA, a2);
        g_mask = (u16)a1 | ((u16)a2<<8);
    }
    void set_mask(u16 mask){
        g_mask = mask;
        Ports::outb(PIC1_DATA, (u8)(mask & 0xFF));
        Ports::outb(PIC2_DATA, (u8)((mask>>8)&0xFF));
    }
    void send_eoi(int irq){
        if (irq >= 8) Ports::outb(PIC2_CMD, 0x20);
        Ports::outb(PIC1_CMD, 0x20);
    }
}
