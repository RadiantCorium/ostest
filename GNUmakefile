# Remove built-in rules and variables
MAKEFLAGS += -rR
.SUFFIXES:

# Name of the final executable
override OUTPUT := ostest

# Convinient macro to reliably declare user overridable vars
override USER_VARIABLE = $(if $(filter $(origin $(1)),default undefined),$(eval override $(1) := $(2)))

# User controllable C compiler command
$(call USER_VARIABLE,KCC,cc)

# User controllable linker command.
$(call USER_VARIABLE,KLD,ld)

# User controllable C flags.
$(call USER_VARIABLE,KCFLAGS,-g -O2 -pipe)

# User controllable C processsor flags, none by default.
$(call USER_VARIABLE,KCPPFLAGS,)

# User controllable nasm flags
$(call USER_VARIABLE,KNASMFLAGS,-F dwarf -g)

# User controllable linker flags, none my default.
$(call USER_VARIABLE,KLDFLAGS,)

# Internal C flags that should NOT be changed by the user.
override KCFLAGS += \
	-Wall \
	-Wextra \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-PIC \
	-m64 \
	-march=x86-64 \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel

# Internal C preprocessor flags that should NOT be changed by the user
override KCPPFLAGS := \
	-I src \
	$(KCPPFLAGS) \
	-MMD \
	-MP

# Internal nasm flags that should NOT be changed by the user
override KNASMFLAGS += \
	-Wall \
	-f elf64

# Internal linker flags that should NOT be changed by the user
override KLDFLAGS += \
	-m elf_x86_64 \
	-nostdlib \
	-static \
	-z max-page-size=0x1000 \
	-T linker.ld

# Use "find" to get all *.c, *.S, and *.asm files in the tree and obtain the object and header dependency file names.
override CFILES := $(shell cd src && find -L * -type f -name '*.c' | LC_ALL=C sort)
override ASFILES := $(shell cd src && find -L * -type f -name '*.S' | LC_ALL=C sort)
override NASMFILES := $(shell cd src && find -L * -type f -name '*.asm' | LC_ALL=C sort)
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o))
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

# Default target
.PHONY: all
all: bin/$(OUTPUT)

# Link rules for final kernel executable
bin/$(OUTPUT): GNUmakefile linker.ld $(OBJ)
	mkdir -p "$$(dirname $@)"
	$(KLD) $(OBJ) $(KLDFLAGS) -o $@

# include header dependencies
-include $(HEADER_DEPS)

# compilation rules for *.c files
obj/%.c.o: src/%.c GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(KCC) $(KCFLAGS) $(KCPPFLAGS) -c $< -o $@

# compilation rules for *.S files
obj/%.S.o: src/%.S GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(KCC) $(KCFLAGS) $(KCPPFLAGS) -c $< -o $@

# compilation rules for *.asm files
obj/%.asm.o: src/%.asm GNUmakefile
	mkdir -p "$$(dirname $@)"
	nasm $(KNASMFLAGS) $< -o $@

# clean command
.PHONY: clean
clean:
	rm -rf bin obj