/* Tell the linker that we want an x86_64 ELF64 output file */
OUTPUT_FORMAT(elf64-x86-64)

/*We want the symbol kmain to be our entry point */
ENTRY(kmain)

/*Define the program headers we want so the bootloader gives us the right MMU permissions
This also allows us to exert more control over the linking process.*/
PHDRS
{
    requests    PT_LOAD;
    text        PT_LOAD;
    rodata      PT_LOAD;
    data        PT_LOAD;
}

SECTIONS
{
    /*We want to be placed in the topmost 2GiB of the address space, for optimizations and because that is what limine spec mandates.*/
    /*Chose 0xffffffff80000000 because it is the beginning of the reggion.*/
    . = 0xffffffff80000000;

    /*Define a section to contain the Limine requests and assign it to its oen PHDR*/
    .requests : {
        KEEP(*(.requests_start_marker))
        KEEP(*(.requests))
        KEEP(*(.requests_end_marker))
    } :requests

    /* Move to the next memory page for .text */
    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .text : {
        *(.text .text.*)
    } :text

    /*Move to the next memory page for .rodata*/
    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    /*Move to the next memory page for .data*/
    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .data : {
        *(.data .data.*)
    } :data

    /* NOTE: .bss needs to be the last thing mapped to :data */
    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } :data

    /* Discard .note.* end .eh_frame* since they may cause issues on some hosts */
    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
    }
}