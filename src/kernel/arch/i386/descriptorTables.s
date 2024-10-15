.section .data
gdtr:
    .word 0
    .long 0

.section .data
idtr:
    .word 0     # Limit storage (16-bit)
    .long 0     # Base storage  (32-bit)

.section .text
.global setGdt
.type setGdt, @function
setGdt:
    lea gdtr, %edx

    movw 4(%esp), %ax
    movw %ax, (%edx)

    movl 8(%esp), %eax
    movl %eax, 2(%edx)
    
    lgdt (%edx)

    ret

.section .text
.global reloadSegments
.type reloadsegments, @function
reloadSegments:
    ljmp $0x08, $.reloadCS

.reloadCS:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    mov $0x10, %ax
    mov %ax, %ss

    mov $stack_top, %esp
    
    ret

.section .text
.global setIdt
.type setIdt, @function
setIdt:
    movw 4(%esp), %ax           # Load limit from stack into AX
    movw %ax, idtr              # Store AX into idtr
    movl 8(%esp), %eax          # Load base from stack into EAX
    movl %eax, idtr + 2         # Store EAX into idtr + 2
    lidt idtr                   # Load IDTR and register with idtr
    ret                         # Return
    