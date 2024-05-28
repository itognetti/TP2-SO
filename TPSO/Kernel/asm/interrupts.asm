GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _syscallHandler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

GLOBAL inb
GLOBAL outb
GLOBAL executeCurrentTask
GLOBAL triggerTimerTick

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscallDispatcher
EXTERN getStackBase
EXTERN enoughTimeLeft
EXTERN nextTask
EXTERN getRSP

SECTION .text
%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	mov rsi, rsp ; pasaje del "vector" de registros
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	pushState
	mov rdi, %1 ; pasaje de ID
	mov rsi, rsp ; pasaje del "vector" de registros
	call exceptionDispatcher
	popState
	call getStackBase
	mov [rsp + 3*8], rax ;rsp a base del stack
	mov rax, 0x400000
	mov [rsp], rax

	iretq
%endmacro

; int 80
_syscallHandler:
	call syscallDispatcher
	iretq

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret

_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn

enable_multitasking:
  mov BYTE [multitaskingEnabled], 1
  jmp tickHandle

;8254 Timer (Timer Tick)
_irq00Handler:
	;irqHandlerMaster 0
  pushState
  cmp BYTE [multitaskingEnabled], 1
  jne enable_multitasking

  call enoughTimeLeft
  cmp rax, 1
  je tickHandle

  switchTask:
  mov rdi, rsp
  mov rsi, ss
  call nextTask
  mov rsp,rax

  tickHandle:
  mov rdi, 0
  call irqDispatcher

  mov al, 20h
  out 20h, al

  popState
  iretq

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Code Exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret

inb:
    push rbp
    mov rbp, rsp
    mov rdx,rdi    ;puerto
    in al,dx       ;valor del puerto
    leave
    ret

outb:
    push rbp
    mov rbp, rsp
    mov rax, rsi     ;valor
    mov rdx, rdi     ;puerto
    out dx, al
    leave
    ret

executeCurrentTask:
  call getRSP
  mov rsp,rax
  popState
  iretq

triggerTimerTick:
  int 20h
  ret

  SECTION .data
  multitaskingEnabled db 0