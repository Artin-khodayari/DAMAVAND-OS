; Multiboot
SECTION .multiboot
align 4
MB_MAGIC  equ 0x1BADB002
MB_FLAGS  equ 0x00000003
MB_CHKSUM equ -(MB_MAGIC + MB_FLAGS)

dd MB_MAGIC
dd MB_FLAGS
dd MB_CHKSUM

SECTION .text
BITS 32
global _start
extern kmain

_start:
    ; set up trivial stack
    mov esp, stack_top
    call kmain
.hang:
    hlt
    jmp .hang

SECTION .bss
align 16
stack_bottom:
    resb 4096
stack_top:
