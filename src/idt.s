global idt_load
section .text
idt_load:
    mov eax, [esp+4]    ; arg: &IDTPointer
    lidt [eax]
    ret
