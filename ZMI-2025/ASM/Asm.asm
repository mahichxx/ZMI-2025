.586
.model flat, stdcall
includelib msvcrtd.lib
includelib ucrtd.lib
includelib vcruntimed.lib
includelib legacy_stdio_definitions.lib
includelib kernel32.lib
includelib "D:\Программирование\3_сем\КПО\ZMI-2025\Debug\InLib.lib"
ExitProcess PROTO :DWORD
outnum PROTO :SDWORD
outstr PROTO :DWORD
newline PROTO
stcmp PROTO :DWORD, :DWORD
strtoint PROTO :DWORD

.stack 4096
.const
	L9	db 'Resuit is', 0
	L18	sbyte 10
	L20	sbyte 5
	L22	sbyte 255
	L25	sbyte 65
	L30	sbyte 15
	L32	db 'Correct', 0
	L34	sbyte 0
	L36	db 'Zero', 0
	L39	db 'Unknown', 0
	L43	sbyte 2
	L46	sbyte 3
.data
	switch_val sdword 0
	res	sbyte 0
	msg	dd 0
	x	sbyte 0
	y	sbyte 0
	hexVal	sbyte 0
	binVal	sbyte 0
	symbol	sbyte 0
	s	dd 0
	result	sbyte 0
.code
main PROC
	mov al, L18
	movsx eax, al
	push eax
	pop eax
	mov x, al
	mov al, L20
	movsx eax, al
	push eax
	pop eax
	mov y, al
	mov al, L22
	movsx eax, al
	push eax
	pop eax
	mov hexVal, al
	mov al, L20
	movsx eax, al
	push eax
	pop eax
	mov binVal, al
	mov al, L25
	movsx eax, al
	push eax
	pop eax
	mov symbol, al
	mov al, x
	movsx eax, al
	push eax
	mov al, y
	movsx eax, al
	push eax
	pop eax
	mov result, al
	pop eax
	mov s, eax
	push s
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, result
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, result
	movsx eax, al
	push eax
	; --- SWITCH START ID:1 ---
	pop eax
	mov switch_val, eax
	mov eax, switch_val
	cmp eax, 15
	je switch_1_case_15
	mov eax, switch_val
	cmp eax, 0
	je switch_1_case_0
	jmp switch_1_default
switch_1_case_15:
	push offset L32
	pop eax
	mov s, eax
	push s
	pop eax
	invoke outnum, eax
	invoke newline
	jmp switch_end_1
switch_1_case_0:
	push offset L36
	pop eax
	mov s, eax
	push s
	pop eax
	invoke outnum, eax
	invoke newline
	jmp switch_end_1
switch_1_default:
	push offset L39
	pop eax
	mov s, eax
	push s
	pop eax
	invoke outnum, eax
	invoke newline
	jmp switch_end_1
	mov al, result
	movsx eax, al
	push eax
	mov al, L43
	movsx eax, al
	push eax
	pop ebx
	pop eax
	imul eax, ebx
	push eax
	pop eax
	mov result, al
	mov al, result
	movsx eax, al
	push eax
	mov al, L46
	movsx eax, al
	push eax
	pop ebx
	pop eax
	cdq
	idiv ebx
	push eax
	pop eax
	mov result, al
	mov al, result
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	jmp switch_end_1
switch_end_1:
	invoke ExitProcess, 0
main ENDP
end main
