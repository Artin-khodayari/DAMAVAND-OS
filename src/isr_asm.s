BITS 32
global idt_load
extern irq_dispatch

idt_load:
    mov  eax, [esp+4]
    lidt [eax]
    ret

%macro MAKE_IRQ 1
global irq_stub%1
irq_stub%1:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push dword %1
    call irq_dispatch
    add esp, 4
    pop gs
    pop fs
    pop es
    pop ds
    popa
    iretd
%endmacro

MAKE_IRQ 0
MAKE_IRQ 1
MAKE_IRQ 2
MAKE_IRQ 3
MAKE_IRQ 4
MAKE_IRQ 5
MAKE_IRQ 6
MAKE_IRQ 7
MAKE_IRQ 8
MAKE_IRQ 9
MAKE_IRQ 10
MAKE_IRQ 11
MAKE_IRQ 12
MAKE_IRQ 13
MAKE_IRQ 14
MAKE_IRQ 15

section .note.GNU-stack noalloc noexec nowrite progbits
