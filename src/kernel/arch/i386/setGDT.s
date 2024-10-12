.section .data
gdtr:
    .word 0     # Limit storage (16-bit)
    .long 0     # Base storage  (32-bit)

.section .text
.global setGdt
.type setGdt, @function
setGdt:
    movw 4(%esp), %ax           # Load limit from stack into AX
    movw %ax, gdtr              # Store AX into gdtr
    movl 8(%esp), %eax          # Load base from stack into EAX
    movl %eax, gdtr + 2         # Store EAX into gdtr + 2
    lgdt gdtr                   # Load GDTR and register with gdtr
    ret                         # Return
    