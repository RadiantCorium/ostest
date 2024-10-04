# Settings
ARCH := i386

SRC_DIR := src
KERNEL_DIR := $(SRC_DIR)/kernel
ARCH_DIR := $(KERNEL_DIR)/arch/$(ARCH)
ISO_NAME := radiance.iso

CFLAGS = -ffreestanding -O2 -Wall -Wextra
CPPFLAGS = -D__is_kernel
INCLUDES = -I./src/libc -I./src/kernel/include -Iinclude
LDFLAGS = -ffreestanding -O2
ASFLAGS = 
LIBS = -nostdlib -lgcc

include $(ARCH_DIR)/make.config

SRC_FILES := $(wildcard $(SRC_DIR)/kernel/*.c) $(wildcard $(SRC_DIR)/libc/*.c)
ASM_FILES := $(wildcard $(SRC_DIR)/kernel/*.s) $(wildcard $(SRC_DIR)/libc/*.c)

ARCH_SRC_FILES := $(wildcard $(ARCH_DIR)/*.c)
ARCH_ASM_FILES := $(wildcard $(ARCH_DIR)/*.s)

LINKER_SCRIPT = $(ARCH_DIR)/linker.ld

OBJS := $(SRC_FILES:.c=.o) $(ASM_FILES:.s:.o) $(ARCH_SRC_FILES:.c=.o) $(ARCH_ASM_FILES:.s=.o)

all: $(OBJS)
	$(CC) -T $(LINKER_SCRIPT) -o kernel.bin $(OBJS) $(LDFLAGS) $(LIBS)

iso: all
	mkdir -p isoroot/boot/grub
	cp kernel.bin isoroot/boot/kernel.bin
	cp grub.cfg isoroot/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_NAME) isoroot

qemu: iso
	qemu-system-$(ARCH) -cdrom radiance.iso

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(LIBS)

%.o: %.s
	$(AS) $< -o $@

clean:
	rm -f $(OBJS) kernel.bin isoroot $(ISO_NAME)

.PHONY: all iso clean