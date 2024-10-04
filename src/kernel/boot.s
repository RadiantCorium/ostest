/* Declare constants for multiboot header. */
.set ALIGN,     1<<0                /* align loade modules on page boundaries */
.set MEMINFO,   1<<1                /* provide memory map */
.set FLAGS,     ALIGN | MEMINFO     /* This is the Multiboot 'flag' field */
.set MAGIC,     0x1BADB002          /* 'magic number' lets bootloader find the header */
.set CHECKSUM,  -(MAGIC + FLAGS)    /* checksum of the above, to prove we are multiboot */

/* Declare a multiboot header that marks the program as a kernel. */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Section to allocate room for a 16KiB stack */
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16KiB
stack_top:

/* _start is the entry point of the kernel.
The bootloader will jump here once the kernel has been loaded. Cannot return from this function as the bootloader is gone. */
.section .text
.global _start
.type _start, @function
_start:
    /* We are now in 32-bit protected mode. All safeguards are disabled and the kernel has full control over the machine. */
    
    /* set the esp register to point to the top of the stack to set it up */
    mov $stack_top, %esp

    /* Enter the high-level kernel */
    call kernel_main

    /* If we somehow end up here (kernel returns) put the computer into an infinite loop */
    cli
1:  hlt
    jmp 1b
.size _start, . - _start