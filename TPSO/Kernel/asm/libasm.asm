GLOBAL cpuVendor
GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHour
GLOBAL getDay
GLOBAL getMonth
GLOBAL getYear
GLOBAL getPressedKey
GLOBAL _RTC
GLOBAL _xadd
GLOBAL _xchg

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

;----------------------------------------------------------------------
; REAL TIME CLOCK (RTC)
;----------------------------------------------------------------------
getSeconds:
    mov rax, 0
	mov al, 0x00
	call _RTC
	ret

getMinutes:
    mov rax, 0
	mov al, 0x02
	call _RTC
	ret

getHour:
	mov rax, 0
	mov al, 0x04
	call _RTC
	ret

getDay:
	mov rax, 0
	mov al, 0x07
	call _RTC
	ret

getMonth:
	mov rax, 0
	mov al, 0x08
	call _RTC
	ret

getYear:
  	mov rax, 0  
	mov al, 0x09
	call _RTC
	ret

_RTC:
	out 70h, al
	in al, 71h
	ret

;----------------------------------------------------------------------
; GETCHAR
;----------------------------------------------------------------------
getPressedKey:
	mov rax, 0
	in al, 60h
	ret

;----------------------------------------------------------------------
; ATOMIC INST
;----------------------------------------------------------------------
_xadd:
	mov rax, rdi
	lock xadd[rsi], eax
	ret

_xchg: 
	mov rax, rsi
	xchg [rdi], eax
	ret