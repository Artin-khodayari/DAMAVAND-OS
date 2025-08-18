# === Tools ===
CC      = g++
ASM     = nasm
LD      = ld

# === Flags ===
CFLAGS  = -m32 -ffreestanding -fno-exceptions -fno-rtti -nostdlib -O2 -Wall -Wextra -c
ASMFLAGS= -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

# === Directories ===
SRC     = src
BUILD   = build

# === Cpp Ojbect Files ===
CPP_OBJS = \
 $(BUILD)/kernel.o $(BUILD)/vga.o $(BUILD)/ports.o \
 $(BUILD)/pic.o $(BUILD)/idt.o $(BUILD)/isr.o $(BUILD)/keyboard.o

# === Assembly Object Files ===
ASM_OBJS = \
 $(BUILD)/boot.o $(BUILD)/hlt.o $(BUILD)/isr_asm.o

OBJS = $(CPP_OBJS) $(ASM_OBJS)

all: DAMAVAND.elf

# === Creating kernel --> .elf ===
DAMAVAND.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/%.o: $(SRC)/%.s
	@mkdir -p $(BUILD)
	$(ASM) $(ASMFLAGS) $< -o $@


# === Run kernel using QEMU ===
run:
	qemu-system-i386 -kernel DAMAVAND.elf

# === Remove files --> Compiled, Assembled & ... ===
clean:
	rm -rf $(BUILD) DAMAVAND.elf
