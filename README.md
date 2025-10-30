# DAMAVAND-OS

[![Boom!](video/DAMAVAND-RECORD-FOR-README.mkv)]

An educational, hobbyist operating system written in **C++** and **x86 assembly**, built completely **from scratch** without external runtime dependencies. Currently runs in **32-bit protected mode** and provides basic VGA text output, keyboard input handling, and interrupt management.

---

## 🛠 Features

* VGA text mode driver (colored text, banners, scrolling)
* Keyboard input handling
* Interrupt handling (IDT + ISR + PIC setup)
* Custom bootloader (written in NASM)
* Minimal kernel written in **C++** (freestanding, no standard library)

---

## 📦 Requirements

To build and run DAMAVAND-OS, you’ll need the following tools on a Debian/Ubuntu-based system:

* `g++` with 32-bit support (installed via `g++-multilib`)
* `nasm` (NetWide Assembler)
* `ld` (from `binutils`)
* `qemu-system-i386` (for running the OS in an emulator)

**Install dependencies:**

```sh
sudo apt update
sudo apt install build-essential g++-multilib nasm binutils qemu-system-i386
```

> Note: `g++-multilib` is required when building 32-bit (`-m32`) binaries on a 64-bit host.

You can use the provided `Makefile` to simplify building and running instead of running many commands manually.

---

## ⚙️ How to build & run

From the repository root:

```sh
# build the kernel
make

# run the resulting kernel image with QEMU
qemu-system-i386 -kernel DAMAVAND.elf

# or use the Makefile convenience target (if provided)
make run
```

**Do not run `make` with `sudo` unless your system configuration specifically requires it.** Running as root can create files owned by root in your workspace which complicates later development.

---

## ❗ Common notes & tips

* If you get compiler errors about `-m32` or missing 32-bit libs, ensure `g++-multilib` and the related multilib packages are installed.
* If `qemu-system-i386` cannot open files due to permissions, fix permissions with `chmod`/`chown` rather than running QEMU as root.
* The kernel image produced by the build is `DAMAVAND.elf`. If your Makefile outputs a differently named file, use that filename in the QEMU command.

---

# 🧑‍💻 About the Developer

This project is developed by [Artin khodayari](https://github.com/Artin-khodayari).

You can contact me and report the problems and bugs to my [Gmail-Account](mailto:ArtinKhodayari2010@gmail.com)

Feel free to reach out for questions, feedback, or collaborations!