GLOBAL _syscall

section .text
_syscall:
    int 80h
    ret

