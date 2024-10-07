# Settings
ARCH := i386

SRC_DIR := src
KERNEL_DIR := $(SRC_DIR)/kernel
ARCH_DIR := $(KERNEL_DIR)/arch/$(ARCH)
ISO_NAME := radiance.iso

CFLAGS = -ffreestanding -O2 -Wall -Wextra
CPPFLAGS = -D__is_kernel
INCLUDES = -I./src/libc -I./src/kernel/include -Iinclude
LDFLAGS = -ffreestanding -O2 -Wl,-Map=linkermap.map -g
ASFLAGS = 
LIBS = -nostdlib -lgcc

include $(ARCH_DIR)/make.config

SRC_FILES := $(wildcard $(SRC_DIR)/kernel/*.c) $(wildcard $(SRC_DIR)/libc/*.c) $(wildcard $(SRC_DIR)/kernel/include/libk/*.c)
ASM_FILES := $(wildcard $(SRC_DIR)/kernel/*.s) $(wildcard $(SRC_DIR)/libc/*.c) $(wildcard $(SRC_DIR)/kernel/include/libk/*.s)

ARCH_SRC_FILES := $(wildcard $(ARCH_DIR)/*.c)
ARCH_ASM_FILES := $(wildcard $(ARCH_DIR)/*.s)

LINKER_SCRIPT = $(ARCH_DIR)/linker.ld

# OBJS := $(SRC_FILES:.c=.o) $(ASM_FILES:.s:.o) $(ARCH_SRC_FILES:.c=.o) $(ARCH_ASM_FILES:.s=.o)
OBJS := $(patsubst $(SRC_DIR)/%.c, build/obj/%.o, $(SRC_FILES)) \
		$(patsubst $(SRC_DIR)/%.s, build/obj/%.o, $(ASM_FILES)) \
		$(patsubst $(ARCH_DIR)/%.c, build/obj/arch/%.o, $(ARCH_SRC_FILES)) \
		$(patsubst $(ARCH_DIR)/%.c, build/obj/arch/%.o, $(ARCH_ASM_FILES)) \

all: $(OBJS)
	$(CC) -T $(LINKER_SCRIPT) -o build/kernel.bin $(OBJS) $(LDFLAGS) $(LIBS) $(INCLUDES)

iso: all
	mkdir -p isoroot/boot/grub
	cp build/kernel.bin isoroot/boot/kernel.bin
	cp grub.cfg isoroot/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_NAME) isoroot

qemu: iso
	qemu-system-$(ARCH) -cdrom radiance.iso

build/obj/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(LIBS)
build/obj/%.o: $(SRC_DIR)/%.s
	mkdir -p $(dir $@)
	$(AS) $< -o $@
build/obj/arch/%.o: $(ARCH_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(LIBS)
build/obj/arch/%.o: $(ARCH_DIR)/%.s
	mkdir -p $(dir $@)
	$(AS) $< -o $@

clean:
	rm -f $(ISO_NAME) linkermap.map
	rm -rf isoroot build

.PHONY: all iso clean