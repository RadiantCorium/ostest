.extern interruptHandler

.section .text
.global isrWrapper
.align 4
.type isrWrapper, @function
isrWrapper:
    pusha
    cld
    call interruptHandler
    popa
    iret