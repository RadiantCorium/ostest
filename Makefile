# Target architecture.
# i386 by default
ARCH ?= i386

include src/kernel/arch/$(ARCH)/make.config

TARGET := i686-elf
CC ?= $(ARCH)-gcc
AS ?= $(ARCH)-as

SRCDIR = src
ARCHDIR = $(SRCDIR)/kernel/arch/$(ARCH)
BUILDDIR = build
ISODIR = isodir

CFLAGS = -ffreestanding -O2 -Wall -Wextra -I./src/libc
LDFLAGS = -T $(ARCHDIR)/linker.ld -ffreestanding -O2 -nostdlib
ASFLAGS = 

ISO = radiance.iso
KERNELBIN = kernel.bin

CSRC = $(wildcard $(SRCDIR)/**/*.c) $(wildcard $(ARCHDIR)/**/*.c)
ASMSRC = $(wildcard $(SRCDIR)/**/*.s) $(wildcard $(ARCHDIR)/**/*.s)

OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(CSRC)) \
	$(patsubst $(SRCDIR)/%.s,$(BUILDDIR)/%.o,$(ASMSRC))

all: $(ISO)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.s | $(BUILDDIR)
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(KERNELBIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) -lgcc

$(ISODIR)/boot/grub/grub.cfg: grub.cfg
	mkdir -p $(ISODIR)/boot/grub
	cp $< $(ISODIR)/boot/grub/grub.cfg

$(ISODIR)/boot/$(KERNELBIN): $(KERNELBIN)
	mkdir -p $(ISODIR)/boot
	cp $< $(ISODIR)/boot/$(KERNELBIN)

$(ISO): $(ISODIR)/boot/grub/grub.cfg $(ISODIR)/boot/$(KERNELBIN)
	grub-mkrescue -o $@ $(ISODIR)

qemu: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILDDIR) $(KERNELBIN) $(ISODIR) $(ISO)


.PHONY: all clean qemu