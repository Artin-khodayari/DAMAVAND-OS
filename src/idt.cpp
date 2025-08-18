#include "idt.hpp"
namespace {
    IDT::Entry idt[256];
    IDT::Ptr   idtp;
}
namespace IDT {
    void set_gate(u8 num, u32 base, u16 sel, u8 flags){
        idt[num].base_lo = (u16)(base & 0xFFFF);
        idt[num].sel     = sel;
        idt[num].always0 = 0;
        idt[num].flags   = flags;
        idt[num].base_hi = (u16)((base>>16)&0xFFFF);
    }
    void init(){
        idtp.limit = sizeof(idt)-1;
        idtp.base  = (u32)&idt[0];
        idt_load(&idtp);
    }
}
