TARGET := i686-elf
CC = $(TARGET)-gcc
AS = $(TARGET)-as

CFLAGS = -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib
ASFLAGS = 

SRCDIR = src
BUILDDIR = build
ISODIR = isodir

ISO = radiance.iso
KERNELBIN = kernel.bin

CSRC = $(wildcard $(SRCDIR)/**/*.c)
ASMSRC = $(wildcard $(SRCDIR)/**/*.s)

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