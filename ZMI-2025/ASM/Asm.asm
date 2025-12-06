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
	L9	db 'StatusOK', 0
	L17	sbyte 10
	L19	sbyte 5
	L21	sbyte -20
	L23	db 'TestOneSimpleMath', 0
	L28	db 'TestTwoLeftLiteral', 0
	L30	sbyte 100
	L34	db 'TestThreeChainCalc', 0
	L41	db 'TestFourComplexParens', 0
	L45	sbyte 2
	L59	db 'TestFiveMixedLogic', 0
	L67	sbyte 3
	L70	db 'TestSixSwitchLogic', 0
	L72	db 'YisFive', 0
	L76	db 'SumIs', 0
	L79	db 'YisTen', 0
	L84	db 'TestSevenHexAndBin', 0
	L86	sbyte 255
	L89	sbyte 7
.data
	switch_val sdword 0
	res	sbyte 0
	s	dd 0
	x	sbyte 0
	y	sbyte 0
	z	sbyte 0
	hard	sbyte 0
	negative	sbyte 0
	info	dd 0
.code
main PROC
	mov al, L17
	movsx eax, al
	push eax
	pop eax
	mov x, al
	mov al, L19
	movsx eax, al
	push eax
	pop eax
	mov y, al
	mov al, L21
	movsx eax, al
	push eax
	pop eax
	mov negative, al
	push offset L23
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, y
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop eax
	mov z, al
	mov al, z
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L28
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, L30
	movsx eax, al
	push eax
	mov al, x
	movsx eax, al
	push eax
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop eax
	mov z, al
	mov al, z
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L34
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, y
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, L17
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, L19
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop eax
	mov z, al
	mov al, z
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L41
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, y
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, L45
	movsx eax, al
	push eax
	pop ebx
	pop eax
	imul eax, ebx
	push eax
	pop eax
	mov hard, al
	mov al, hard
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, y
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, L19
	movsx eax, al
	push eax
	mov al, L45
	movsx eax, al
	push eax
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop ebx
	pop eax
	cdq
	idiv ebx
	push eax
	pop eax
	mov hard, al
	mov al, hard
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, x
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, y
	movsx eax, al
	push eax
	mov al, L45
	movsx eax, al
	push eax
	pop ebx
	pop eax
	imul eax, ebx
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, L45
	movsx eax, al
	push eax
	pop ebx
	pop eax
	cdq
	idiv ebx
	push eax
	pop eax
	mov hard, al
	mov al, hard
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L59
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, L17
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, y
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop eax
	mov z, al
	mov al, z
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, L45
	movsx eax, al
	push eax
	mov al, x
	movsx eax, al
	push eax
	mov al, L67
	movsx eax, al
	push eax
	pop ebx
	pop eax
	imul eax, ebx
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop eax
	mov z, al
	mov al, z
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L70
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, y
	movsx eax, al
	push eax
	; --- SWITCH START ID:1 ---
	pop eax
	mov switch_val, eax
	mov eax, switch_val
	cmp eax, 5
	je switch_1_case_5
	mov eax, switch_val
	cmp eax, 10
	je switch_1_case_10
	jmp switch_1_default
switch_1_case_5:
	push offset L72
	pop eax
	mov info, eax
	push info
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, y
	movsx eax, al
	push eax
	pop eax
	mov z, al
	push offset L76
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, z
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	jmp switch_end_1
switch_1_case_10:
	push offset L79
	pop eax
	mov info, eax
	push info
	pop eax
	invoke outnum, eax
	invoke newline
	jmp switch_end_1
switch_1_default:
	pop eax
	mov info, eax
	push info
	pop eax
	invoke outnum, eax
	invoke newline
	jmp switch_end_1
	push offset L84
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, L86
	movsx eax, al
	push eax
	pop eax
	mov z, al
	mov al, z
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, L89
	movsx eax, al
	push eax
	pop eax
	mov z, al
	mov al, z
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
