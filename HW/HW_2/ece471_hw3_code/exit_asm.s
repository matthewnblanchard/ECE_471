
@ Syscall defines
.equ SYSCALL_EXIT,     1


        .globl _start
_start:

        @================================
        @ Exit
        @================================
exit:
        mov r0, #42
        mov r7, #SYSCALL_EXIT
        swi 0x0
