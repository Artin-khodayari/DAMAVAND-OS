# DAMAVAND-OS

An educational, hobbyist operating system written in **C++** and **x86 assembly**, built completely **from scratch** without dependencies.  
Currently runs in **32-bit protected mode** and provides basic VGA text output, keyboard input handling, and interrupt management.

---

## 🛠 Features
- VGA text mode driver (colored text, banners, scrolling)
- Keyboard input handling
- Interrupt handling (IDT + ISR + PIC setup)
- Custom bootloader (written in NASM)
- Minimal kernel written in **C++ (freestanding, no standard library)**

---

## 📦 Requirements
To build and run DAMAVAND-OS, you’ll need:

- `g++` (with i386 support, for 32-bit compilation)
- `nasm` (NetWide assembler)
- `ld` (binutils linker)
- `qemu-system-i386` (for running the OS)

On Ubuntu/Debian:
```sh
sudo apt update
sudo apt install g++ nasm binutils qemu-system-x86
