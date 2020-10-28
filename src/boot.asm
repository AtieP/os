section .stivalehdr
stivale_header:
	.stack: dq stack.top
	.flags: dw 1
	.framebuffer_width: dw 0
	.framebuffer_height: dw 0
	.framebuffer_bpp: dw 0
	.entry_point: dq _start

section .bss
stack:
	resb 4096
.top:

section .data
global GDT64
global GDT64_PTR

GDT64:
	.null: equ $ - GDT64
		dq 0
	.code: equ $ - GDT64
		dw 0
		dw 0
		db 0
		db 10011010b
		db 00100000b
		db 0
	.data: equ $ - GDT64
		dw 0
		dw 0
		db 0
		db 10010010b
		db 0
		db 0
GDT64_END:

GDT64_PTR:
	dw GDT64_END - GDT64 - 1 	; Limit
	dq GDT64					; Base

section .text
extern kmain
global _start
_start:
	cli

	lgdt [GDT64_PTR]

	mov rax, rsp

	push 0x10
	push rax
	pushf
	push 0x08
	push .load_kernel
	iretq

.load_kernel:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov gs, ax
	mov fs, ax

	call kmain

hang:
	hlt
	jmp hang