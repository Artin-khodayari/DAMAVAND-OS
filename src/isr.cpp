#include "isr.hpp"
#include "idt.hpp"
#include "pic.hpp"

namespace ISR {
    static irq_handler_t handlers[16] = {0};

    extern "C" {
        void irq_stub0();  void irq_stub1();  void irq_stub2();  void irq_stub3();
        void irq_stub4();  void irq_stub5();  void irq_stub6();  void irq_stub7();
        void irq_stub8();  void irq_stub9();  void irq_stub10(); void irq_stub11();
        void irq_stub12(); void irq_stub13(); void irq_stub14(); void irq_stub15();
        void irq_dispatch(int irq);
    }

    extern "C" void irq_dispatch(int irq){
        if (irq >=0 && irq <16 && handlers[irq]) handlers[irq]();
        PIC::send_eoi(irq);
    }

    void register_irq(int n, irq_handler_t h){
        if (n>=0 && n<16) handlers[n] = h;
    }

    void unmask_only_keyboard(){
        PIC::set_mask(0xFFFD); // all 1 except bit1=0 (IRQ1 enabled)
    }

    void install(){
        PIC::remap(); // map IRQs to 0x20..0x2F

        IDT::set_gate(32, (u32)irq_stub0,  0x08, 0x8E);
        IDT::set_gate(33, (u32)irq_stub1,  0x08, 0x8E);
        IDT::set_gate(34, (u32)irq_stub2,  0x08, 0x8E);
        IDT::set_gate(35, (u32)irq_stub3,  0x08, 0x8E);
        IDT::set_gate(36, (u32)irq_stub4,  0x08, 0x8E);
        IDT::set_gate(37, (u32)irq_stub5,  0x08, 0x8E);
        IDT::set_gate(38, (u32)irq_stub6,  0x08, 0x8E);
        IDT::set_gate(39, (u32)irq_stub7,  0x08, 0x8E);
        IDT::set_gate(40, (u32)irq_stub8,  0x08, 0x8E);
        IDT::set_gate(41, (u32)irq_stub9,  0x08, 0x8E);
        IDT::set_gate(42, (u32)irq_stub10, 0x08, 0x8E);
        IDT::set_gate(43, (u32)irq_stub11, 0x08, 0x8E);
        IDT::set_gate(44, (u32)irq_stub12, 0x08, 0x8E);
        IDT::set_gate(45, (u32)irq_stub13, 0x08, 0x8E);
        IDT::set_gate(46, (u32)irq_stub14, 0x08, 0x8E);
        IDT::set_gate(47, (u32)irq_stub15, 0x08, 0x8E);

        IDT::init();
    }
}
